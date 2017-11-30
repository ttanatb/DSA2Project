#include "MyEntity.h"
using namespace Simplex;
std::map<String, MyEntity*> MyEntity::m_IDMap;

#pragma region Accessors
//  Accessors
matrix4 Simplex::MyEntity::GetModelMatrix(void) { return m_m4ToWorld; }
// physics accessors
vector3 Simplex::MyEntity::GetForward(void) { return forward; }
vector3 Simplex::MyEntity::GetPosition(void) { return position; }
vector3 Simplex::MyEntity::GetVelocity(void) { return velocity; }
vector3 Simplex::MyEntity::GetAcceleration(void) { return acceleration; }

// setters
void Simplex::MyEntity::SetPosition(vector3 position) { 
    this->position = position;
}
void Simplex::MyEntity::SetPosition(float xPos, float yPos, float zPos) {
    this->position = vector3(xPos, yPos, zPos);
}
void Simplex::MyEntity::SetVelocity(vector3 velocity) {
    this->velocity = velocity;
}
void Simplex::MyEntity::SetVelocity(float xVelocity, float yVelocity, float zVelocity) {
    this->velocity = vector3(xVelocity, yVelocity, zVelocity);
}
void Simplex::MyEntity::SetForward(vector3 forward) {
    this->forward = forward;
}
void Simplex::MyEntity::SetRotation(quaternion rotation) {
    this->rotation = rotation;
}
void Simplex::MyEntity::AddForce(vector3 force) {
    this->acceleration = acceleration + (force / mass);
}


void Simplex::MyEntity::SetModelMatrix(matrix4 a_m4ToWorld)
{
	if (!m_bInMemory)
		return;

	m_m4ToWorld = a_m4ToWorld;
	m_pModel->SetModelMatrix(m_m4ToWorld);
	m_pRigidBody->SetModelMatrix(m_m4ToWorld);
}
Model* Simplex::MyEntity::GetModel(void) { return m_pModel; }
MyRigidBody* Simplex::MyEntity::GetRigidBody(void) { return m_pRigidBody; }
bool Simplex::MyEntity::IsInitialized(void) { return m_bInMemory; }
String Simplex::MyEntity::GetUniqueID(void) { return m_sUniqueID; }
void Simplex::MyEntity::SetAxisVisible(bool a_bSetAxis) { m_bSetAxis = a_bSetAxis; }

MyEntity* Simplex::MyEntity::GetEntity(String a_sUniqueID)
{
	//look the entity based on the unique id
	auto entity = m_IDMap.find(a_sUniqueID);
	//if not found return nullptr, if found return it
	return entity == m_IDMap.end() ? nullptr : entity->second;
}
#pragma endregion

#pragma region Big Three & Helper Functions
//  MyEntity
void Simplex::MyEntity::Init(void)
{
	m_pMeshMngr = MeshManager::GetInstance();
	m_bInMemory = false;
	m_bSetAxis = false;
	m_pModel = nullptr;
	m_pRigidBody = nullptr;
	m_DimensionArray = nullptr;
	m_m4ToWorld = IDENTITY_M4;
	m_sUniqueID = "";
	m_nDimensionCount = 0;
  position = vector3(0);
  velocity = vector3(0);
  acceleration = vector3(0);
  forward = vector3(0);
  mass = 1;
}
void Simplex::MyEntity::Swap(MyEntity& other)
{
	m_bInMemory = false;
	std::swap(m_pModel, other.m_pModel);
	std::swap(m_pRigidBody, other.m_pRigidBody);
	std::swap(m_m4ToWorld, other.m_m4ToWorld);
	std::swap(m_pMeshMngr, other.m_pMeshMngr);
	std::swap(m_bInMemory, other.m_bInMemory);
	std::swap(m_sUniqueID, other.m_sUniqueID);
	std::swap(m_bSetAxis, other.m_bSetAxis);
	std::swap(m_nDimensionCount, other.m_nDimensionCount);
	std::swap(m_DimensionArray, other.m_DimensionArray);
}
void Simplex::MyEntity::Release(void)
{
	m_pMeshMngr = nullptr;
	//it is not the job of the entity to release the model, 
	//it is for the mesh manager to do so.
	m_pModel = nullptr;
	if (m_DimensionArray)
	{
		delete[] m_DimensionArray;
		m_DimensionArray = nullptr;
	}
	SafeDelete(m_pRigidBody);
	m_IDMap.erase(m_sUniqueID);
}
//The big 3
Simplex::MyEntity::MyEntity(String a_sFileName, String a_sUniqueID)
{
	Init();
	m_pModel = new Model();
	m_pModel->Load(a_sFileName);
	//if the model is loaded
	if (m_pModel->GetName() != "")
	{
		GenUniqueID(a_sUniqueID);
		m_sUniqueID = a_sUniqueID;
		m_IDMap[a_sUniqueID] = this;
		m_pRigidBody = new MyRigidBody(m_pModel->GetVertexList()); //generate a rigid body
		m_bInMemory = true; //mark this entity as viable
	}
}
Simplex::MyEntity::MyEntity(MyEntity const& other)
{
	m_bInMemory = other.m_bInMemory;
	m_pModel = other.m_pModel;
	//generate a new rigid body we do not share the same rigid body as we do the model
	m_pRigidBody = new MyRigidBody(m_pModel->GetVertexList());
	m_m4ToWorld = other.m_m4ToWorld;
	m_pMeshMngr = other.m_pMeshMngr;
	m_sUniqueID = other.m_sUniqueID;
	m_bSetAxis = other.m_bSetAxis;
	m_nDimensionCount = other.m_nDimensionCount;
	m_DimensionArray = other.m_DimensionArray;

}
Simplex::MyEntity::MyEntity() { }
MyEntity& Simplex::MyEntity::operator=(MyEntity const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyEntity temp(other);
		Swap(temp);
	}
	return *this;
}
MyEntity::~MyEntity() { Release(); }

#pragma endregion

//--- Methods
#pragma region Collision & Dimensions
void Simplex::MyEntity::AddDimension(uint a_uDimension)
{
	//we need to check that this dimension is not already allocated in the list
	if (IsInDimension(a_uDimension))
		return;//it is, so there is no need to add

			   //insert the entry
	uint* pTemp;
	pTemp = new uint[m_nDimensionCount + 1];
	if (m_DimensionArray)
	{
		memcpy(pTemp, m_DimensionArray, sizeof(uint) * m_nDimensionCount);
		delete[] m_DimensionArray;
		m_DimensionArray = nullptr;
	}
	pTemp[m_nDimensionCount] = a_uDimension;
	m_DimensionArray = pTemp;

	++m_nDimensionCount;

	SortDimensions();
}
void Simplex::MyEntity::RemoveDimension(uint a_uDimension)
{
	//if there are no dimensions return
	if (m_nDimensionCount == 0)
		return;

	//we look one by one if its the one wanted
	for (uint i = 0; i < m_nDimensionCount; i++)
	{
		if (m_DimensionArray[i] == a_uDimension)
		{
			//if it is, then we swap it with the last one and then we pop
			std::swap(m_DimensionArray[i], m_DimensionArray[m_nDimensionCount - 1]);
			uint* pTemp;
			pTemp = new uint[m_nDimensionCount - 1];
			if (m_DimensionArray)
			{
				memcpy(pTemp, m_DimensionArray, sizeof(uint) * (m_nDimensionCount - 1));
				delete[] m_DimensionArray;
				m_DimensionArray = nullptr;
			}
			m_DimensionArray = pTemp;

			--m_nDimensionCount;
			SortDimensions();
			return;
		}
	}
}
void Simplex::MyEntity::ClearDimensionSet(void)
{
	if (m_DimensionArray)
	{
		delete[] m_DimensionArray;
		m_DimensionArray = nullptr;
	}
	m_nDimensionCount = 0;
}

bool Simplex::MyEntity::IsInDimension(uint a_uDimension)
{
	//see if the entry is in the set
	for (uint i = 0; i < m_nDimensionCount; i++)
	{
		if (m_DimensionArray[i] == a_uDimension)
			return true;
	}
	return false;
}
bool Simplex::MyEntity::SharesDimension(MyEntity* const a_pOther)
{
	//special case: if there are no dimensions on either MyEntity
	//then they live in the special global dimension
	if (0 == m_nDimensionCount)
	{
		//if no spatial optimization all cases should fall here as every 
		//entity is by default, under the special global dimension only
		if (0 == a_pOther->m_nDimensionCount)
			return true;
	}

	//for each dimension on both Entities we check if there is a common dimension
	for (uint i = 0; i < m_nDimensionCount; ++i)
	{
		for (uint j = 0; j < a_pOther->m_nDimensionCount; j++)
		{
			if (m_DimensionArray[i] == a_pOther->m_DimensionArray[j])
				return true; //as soon as we find one we know they share dimensionality
		}
	}

	//could not find a common dimension
	return false;
}
void Simplex::MyEntity::SortDimensions(void)
{
	std::sort(m_DimensionArray, m_DimensionArray + m_nDimensionCount);
}

bool Simplex::MyEntity::IsColliding(MyEntity* const other)
{
	//if not in memory return
	if (!m_bInMemory || !other->m_bInMemory)
		return true;

	//if the entities are not living in the same dimension
	//they are not colliding
	if (!SharesDimension(other))
		return false;

	return m_pRigidBody->IsColliding(other->GetRigidBody());
}
void Simplex::MyEntity::ClearCollisionList(void)
{
	m_pRigidBody->ClearCollidingList();
}
#pragma endregion

void Simplex::MyEntity::AddToRenderList(bool a_bDrawRigidBody)
{
	//if not in memory return
	if (!m_bInMemory)
		return;

	//draw model
	m_pModel->AddToRenderList();

	//draw rigid body
	if (a_bDrawRigidBody)
		m_pRigidBody->AddToRenderList();

	if (m_bSetAxis)
		m_pMeshMngr->AddAxisToRenderList(m_m4ToWorld);
}

void Simplex::MyEntity::GenUniqueID(String& a_sUniqueID)
{
	static uint index = 0;
	String sName = a_sUniqueID;
	MyEntity* pEntity = GetEntity(a_sUniqueID);
	//while MyEntity exists keep changing name
	while (pEntity)
	{
		a_sUniqueID = sName + "_" + std::to_string(index);
		index++;
		pEntity = GetEntity(a_sUniqueID);
	}
	return;
}

void Simplex::MyEntity::Update(void)
{
	if (!m_bInMemory)
		return;

	//To-do:
	//update physics

  // add to velocity
  velocity = velocity + acceleration;
  // add to position
  position = position + velocity;

  // get rotation to forward vector
  /*
  rotation = glm::conjugate(glm::toQuat(
      glm::lookAt(glm::vec3(forward.x, forward.y, forward.z),
          glm::vec3(forward.x, forward.y, forward.z) + velocity,
          glm::vec3(0, 1, 0)
      )
  ));
  */
   vector3 w = glm::cross(AXIS_Z, forward);
   quaternion q = quaternion(glm::dot(forward, velocity), w.x, w.y, w.z);
   // sqrt ( a2 + b2+c2+ d2)
   q.w = glm::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
   rotation = glm::normalize(q);

   if (glm::length2(velocity) > 0.0f)
	forward = glm::normalize(velocity);


  // update entity positions
  m_m4ToWorld = glm::translate(IDENTITY_M4, position) * ToMatrix4(rotation);
  SetModelMatrix(m_m4ToWorld);

  // reset acceleration
  acceleration = vector3(0);

  /*Current plan is update function adds acceleration to velocity.
  Add velocity to position.
  Update the model position using entity position.
  Set acceleration to zero.

Then for collision resolution, we can access each object's acceleration and set them equal and opposite (or whatever we need), and it should give them a nice bounce.*/
	//no force/acceleration, just velocity, position
	//google a way to calculate the rotation quaternion based off of the normalized velocity
	//the model matrix should also change
}