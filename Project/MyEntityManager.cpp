#include "MyEntityManager.h"
using namespace Simplex;
//  MyEntityManager
Simplex::MyEntityManager* Simplex::MyEntityManager::m_pInstance = nullptr;
void Simplex::MyEntityManager::Init(void)
{
	m_uEntityCount = 0;
	m_mEntityArray = nullptr;
}
void Simplex::MyEntityManager::Release(void)
{

	for (uint uEntity = 0; uEntity < m_uEntityCount; ++uEntity)
	{
		MyEntity* pEntity = m_mEntityArray[uEntity];
		SafeDelete(pEntity);
	}
	m_uEntityCount = 0;
	m_mEntityArray = nullptr;
}
Simplex::MyEntityManager* Simplex::MyEntityManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyEntityManager();
	}
	return m_pInstance;
}
void Simplex::MyEntityManager::ReleaseInstance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	//look one by one for the specified unique id
	for (uint uIndex = 0; uIndex < m_uEntityCount; ++uIndex)
	{
		if (a_sUniqueID == m_mEntityArray[uIndex]->GetUniqueID())
			return uIndex;
	}
	//if not found return -1
	return -1;
}
//Accessors
Simplex::uint Simplex::MyEntityManager::GetEntityCount(void) { return m_uZombieCount + m_uBallCount + m_uWallCount; }
Simplex::Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return nullptr;

	// if out of bounds
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex]->GetModel();
}
Simplex::Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		return pTemp->GetModel();
	}
	return nullptr;
}
Simplex::MyRigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	//if the list is empty return
	if (GetEntityCount() == 0)
		return nullptr;

	// if out of bounds
	if (a_uIndex >= GetEntityCount())
		a_uIndex = GetEntityCount() - 1;

	if (a_uIndex < m_uWallCount) {
		return m_pWallArray[a_uIndex]->GetRigidBody();
	}
	else if (a_uIndex - m_uWallCount < m_uZombieCount)
	{
		return m_pZombieArray[a_uIndex - m_uWallCount]->GetRigidBody();
	}
	else
	{
		return m_pBallArray[a_uIndex - m_uWallCount - m_uZombieCount]->GetRigidBody();
	}

	//return m_mEntityArray[a_uIndex]->GetRigidBody();
}
Simplex::MyRigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		return pTemp->GetRigidBody();
	}
	return nullptr;
}
Simplex::matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return matrix4();

	// if out of bounds
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex]->GetModelMatrix();
}
Simplex::matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		return pTemp->GetModelMatrix();
	}
	return IDENTITY_M4;
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		pTemp->SetModelMatrix(a_m4ToWorld);
	}
}
void Simplex::MyEntityManager::SetAxisVisibility(bool a_bVisibility, uint a_uIndex)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex]->SetAxisVisible(a_bVisibility);
}
void Simplex::MyEntityManager::SetAxisVisibility(bool a_bVisibility, String a_sUniqueID)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		pTemp->SetAxisVisible(a_bVisibility);
	}
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	m_mEntityArray[a_uIndex]->SetModelMatrix(a_m4ToWorld);
}
//The big 3
Simplex::MyEntityManager::MyEntityManager() { Init(); }
Simplex::MyEntityManager::MyEntityManager(MyEntityManager const& a_pOther) { }
Simplex::MyEntityManager& Simplex::MyEntityManager::operator=(MyEntityManager const& a_pOther) { return *this; }
Simplex::MyEntityManager::~MyEntityManager() { Release(); };
// other methods
void Simplex::MyEntityManager::Update(void)
{
	if (m_pFloor != nullptr)
		m_pFloor->Update();

	//update all balls
	for (uint j = 0; j < m_uBallCount; ++j)
	{
		//clear & update
		m_pBallArray[j]->Update();
		m_pBallArray[j]->ClearCollisionList();
	}


	for (uint j = 0; j < m_uZombieCount; ++j)
	{
		//clear & update
		m_pZombieArray[j]->Update();
		m_pZombieArray[j]->ClearCollisionList();
	}

	//wall collisions and clearing collision lists and updating
	for (uint i = 0; i < m_uWallCount; ++i)
	{
		//m_pWallArray[i]->Update(); //this shouldn't really do anything 
		m_pWallArray[i]->ClearCollisionList();

		for (uint j = 0; j < m_uBallCount; ++j) {
			//check ball vs wall
			if (m_pWallArray[i]->IsColliding(m_pBallArray[j]))
				m_pBallArray[j]->Resolve(m_pWallArray[i]);
		}

		//update all zombies
		for (uint j = 0; j < m_uZombieCount; ++j)
		{
			//check zombie vs wall
			if (m_pWallArray[i]->IsColliding(m_pZombieArray[j]))
				m_pZombieArray[j]->Resolve(m_pWallArray[i]);
		}
	}

	//ball collision
	for (uint i = 0; i < m_uBallCount; ++i)
	{
		//ball vs ball
		if (i < m_uBallCount - 1)
		{
			for (uint j = i + 1; j < m_uBallCount; ++j)
			{
				if (m_pBallArray[i]->IsColliding(m_pBallArray[j]))
					m_pBallArray[i]->Resolve(m_pBallArray[j]);
			}
		}

		//ball vs zombie
		for (uint j = 0; j < m_uZombieCount; ++j)
		{
			if (m_pBallArray[i]->IsColliding(m_pZombieArray[j]))
				m_pZombieArray[j]->Resolve(m_pBallArray[i]);
		}
	}

	//zombie collisions
	if (m_uZombieCount == 0) return; //it will loop through all uints if zombie count is 0
	for (uint i = 0; i < m_uZombieCount - 1; ++i)
	{
		for (uint j = i + 1; j < m_uZombieCount; ++j) //zombie vs zombie 
		{
			if (m_pZombieArray[i]->IsColliding(m_pZombieArray[j]))
				m_pZombieArray[i]->Resolve(m_pZombieArray[j]);
		}
	}
}
void Simplex::MyEntityManager::AddEntity(String a_sFileName, String a_sUniqueID)
{
	//Create a temporal entity to store the object
	MyEntity* pTemp = new MyEntity(a_sFileName, a_sUniqueID);
	//if I was able to generate it add it to the list
	if (pTemp->IsInitialized())
	{
		//create a new temp array with one extra entry
		PEntity* tempArray = new PEntity[m_uEntityCount + 1];
		//start from 0 to the current count
		uint uCount = 0;
		for (uint i = 0; i < m_uEntityCount; ++i)
		{
			tempArray[uCount] = m_mEntityArray[i];
			++uCount;
		}
		tempArray[uCount] = pTemp;
		//if there was an older array delete
		if (m_mEntityArray)
		{
			delete[] m_mEntityArray;
		}
		//make the member pointer the temp pointer
		m_mEntityArray = tempArray;
		//add one entity to the count
		++m_uEntityCount;
	}
}

void Simplex::MyEntityManager::AddZombie(vector3 a_v3Pos)
{
	Zombie* pTemp = new Zombie();
	pTemp->Initialize(a_v3Pos);
	if (pTemp->IsInitialized())
	{
		//create a new temp array with one extra entry
		PZombie* tempArray = new PZombie[m_uZombieCount + 1];
		//start from 0 to the current count
		uint uCount = 0;
		for (uint i = 0; i < m_uZombieCount; ++i)
		{
			tempArray[uCount] = m_pZombieArray[i];
			++uCount;
		}
		tempArray[uCount] = pTemp;

		//if there was an older array delete
		if (m_pZombieArray)
		{
			delete[] m_pZombieArray;
		}

		//make the member pointer the temp pointer
		m_pZombieArray = tempArray;
		//add one entity to the count
		++m_uZombieCount;
	}
}

void Simplex::MyEntityManager::AddWall(vector3 position, vector3 forward, vector3 scale)
{
	Wall* pTemp = new Wall();
	pTemp->Initialize(position, forward, scale);
	if (pTemp->IsInitialized())
	{
		//create a new temp array with one extra entry
		PWall* tempArray = new PWall[m_uWallCount + 1];
		//start from 0 to the current count
		uint uCount = 0;
		for (uint i = 0; i < m_uWallCount; ++i)
		{
			tempArray[uCount] = m_pWallArray[i];
			++uCount;
		}
		tempArray[uCount] = pTemp;

		//if there was an older array delete
		if (m_pWallArray)
		{
			delete[] m_pWallArray;
		}

		//make the member pointer the temp pointer
		m_pWallArray = tempArray;
		//add one entity to the count
		++m_uWallCount;
	}
}

void Simplex::MyEntityManager::AddFloor()
{
	//Create a temporal entity to store the object
	MyEntity* pTemp = new MyEntity("Floor.obj", "floor");
	//if I was able to generate it add it to the list
	if (pTemp->IsInitialized())
	{
		if (m_pFloor != nullptr)
			delete(m_pFloor);

		m_pFloor = pTemp;
	}
}

void Simplex::MyEntityManager::AddBall(vector3 position, vector3 forward, float speed)
{
	BouncyBall* pTemp = new BouncyBall();
	pTemp->Initialize(position, forward, speed);
	if (pTemp->IsInitialized())
	{
		//create a new temp array with one extra entry
		PBouncyBall* tempArray = new PBouncyBall[m_uBallCount + 1];
		//start from 0 to the current count
		uint uCount = 0;
		for (uint i = 0; i < m_uBallCount; ++i)
		{
			tempArray[uCount] = m_pBallArray[i];
			++uCount;
		}
		tempArray[uCount] = pTemp;

		//if there was an older array delete
		if (m_pBallArray)
		{
			delete[] m_pBallArray;
		}

		//make the member pointer the temp pointer
		m_pBallArray = tempArray;
		//add one entity to the count
		++m_uBallCount;
	}
}

void Simplex::MyEntityManager::ClearZombies(void)
{
	if (m_pZombieArray != nullptr)
		delete[] m_pZombieArray;

	m_uZombieCount = 0;
	m_pZombieArray = nullptr;
}

void Simplex::MyEntityManager::ClearBalls(void)
{
	if (m_pBallArray != nullptr)
		delete[] m_pBallArray;

	m_uBallCount = 0;
	m_pBallArray = nullptr;
}

bool Simplex::MyEntityManager::CheckZombieWin(vector3 position, float size)
{
	for (uint i = 0; i < m_uZombieCount; ++i) {
		vector3 zombPos = m_pZombieArray[i]->GetPosition();
		float zombSize = m_pZombieArray[i]->GetRigidBody()->GetRadius();
		if (zombPos.z + zombSize > position.z - size &&
			zombPos.x + zombSize > position.x - size &&
			zombPos.x - zombSize < position.x + size)
		{
			return true;
		}
	}
	return false;
}

void Simplex::MyEntityManager::SetTestZombieModelMatrix(matrix4 a_m4ToWorld)
{
	if (m_uZombieCount > 0)
		m_pZombieArray[m_uZombieCount - 1]->SetModelMatrix(a_m4ToWorld);
}

void Simplex::MyEntityManager::SetTestModelMatrix(matrix4 a_m4ToWorld)
{
	if (m_uBallCount > 0)
		m_pBallArray[0]->SetModelMatrix(a_m4ToWorld);
}

void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return;

	// if out of bounds choose the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	// if the entity is not the very last we swap it for the last one
	if (a_uIndex != m_uEntityCount - 1)
	{
		std::swap(m_mEntityArray[a_uIndex], m_mEntityArray[m_uEntityCount - 1]);
	}

	//and then pop the last one
	//create a new temp array with one less entry
	PEntity* tempArray = new PEntity[m_uEntityCount - 1];
	//start from 0 to the current count
	for (uint i = 0; i < m_uEntityCount - 1; ++i)
	{
		tempArray[i] = m_mEntityArray[i];
	}
	//if there was an older array delete
	if (m_mEntityArray)
	{
		delete[] m_mEntityArray;
	}
	//make the member pointer the temp pointer
	m_mEntityArray = tempArray;
	//add one entity to the count
	--m_uEntityCount;
}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{
	int nIndex = GetEntityIndex(a_sUniqueID);
	RemoveEntity((uint)nIndex);
}
Simplex::String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return "";

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex]->GetUniqueID();
}
Simplex::MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return nullptr;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex];
}
void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{
	//if out of bounds will do it for all
	if (a_uIndex >= m_uEntityCount)
	{
		//add for each one in the entity list
		for (a_uIndex = 0; a_uIndex < m_uEntityCount; ++a_uIndex)
		{
			m_mEntityArray[a_uIndex]->AddToRenderList(a_bRigidBody);
		}
	}
	else //do it for the specified one
	{
		m_mEntityArray[a_uIndex]->AddToRenderList(a_bRigidBody);
	}
}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		pTemp->AddToRenderList(a_bRigidBody);
	}
}
void Simplex::MyEntityManager::AddAllToRenderList(bool a_bRigidBody)
{
	for (uint i = 0; i < m_uZombieCount; ++i) {
		m_pZombieArray[i]->AddToRenderList(a_bRigidBody);
	}

	for (uint i = 0; i < m_uWallCount; ++i) {
		m_pWallArray[i]->AddToRenderList(a_bRigidBody);
	}

	for (uint i = 0; i < m_uBallCount; ++i) {
		m_pBallArray[i]->AddToRenderList(a_bRigidBody);
	}

	if (m_pFloor != nullptr)
		m_pFloor->AddToRenderList(a_bRigidBody);
}
void Simplex::MyEntityManager::AddDimension(uint a_uIndex, uint a_uDimension)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex]->AddDimension(a_uDimension);
}
void Simplex::MyEntityManager::AddDimension(String a_sUniqueID, uint a_uDimension)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		pTemp->AddDimension(a_uDimension);
	}
}
void Simplex::MyEntityManager::RemoveDimension(uint a_uIndex, uint a_uDimension)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex]->RemoveDimension(a_uDimension);
}
void Simplex::MyEntityManager::RemoveDimension(String a_sUniqueID, uint a_uDimension)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		pTemp->RemoveDimension(a_uDimension);
	}
}
void Simplex::MyEntityManager::ClearDimensionSetAll(void)
{
	for (uint i = 0; i < m_uEntityCount; ++i)
	{
		ClearDimensionSet(i);
	}
}
void Simplex::MyEntityManager::ClearDimensionSet(uint a_uIndex)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex]->ClearDimensionSet();
}
void Simplex::MyEntityManager::ClearDimensionSet(String a_sUniqueID)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		pTemp->ClearDimensionSet();
	}
}
bool Simplex::MyEntityManager::IsInDimension(uint a_uIndex, uint a_uDimension)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return false;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex]->IsInDimension(a_uDimension);
}
bool Simplex::MyEntityManager::IsInDimension(String a_sUniqueID, uint a_uDimension)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		return pTemp->IsInDimension(a_uDimension);
	}
	return false;
}
bool Simplex::MyEntityManager::SharesDimension(uint a_uIndex, MyEntity* const a_pOther)
{
	//if the list is empty return
	if (m_uEntityCount == 0)
		return false;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_mEntityArray[a_uIndex]->SharesDimension(a_pOther);
}
bool Simplex::MyEntityManager::SharesDimension(String a_sUniqueID, MyEntity* const a_pOther)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		return pTemp->SharesDimension(a_pOther);
	}
	return false;
}