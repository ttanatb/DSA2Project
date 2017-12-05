#include "Zombie.h"
using namespace Simplex;

Simplex::Zombie::Zombie()
{
	Init();
	m_pModel = new Model();
	m_pModel->Load(FILE_PATH);

	//if the model is loaded
	if (m_pModel->GetName() != "")
	{
		GenUniqueID(defaultID);
		m_sUniqueID = defaultID;
		m_IDMap[defaultID] = this;
		m_pRigidBody = new MyRigidBody(m_pModel->GetVertexList()); //generate a rigid body

		std::vector<vector3> vectorList = std::vector<vector3>();
		vectorList.push_back(vector3(.26f, 1.50f, .25f));
		vectorList.push_back(vector3(-.26f, 1.9f, -.18f));
		m_pHeadRB = new MyRigidBody(vectorList);

		vectorList.clear();
		vectorList.push_back(vector3(.28f, 1.50f, .28f));
		vectorList.push_back(vector3(-.28f, 0.8f, -.12f));
		m_pTorsoRB = new MyRigidBody(vectorList);

		vectorList.clear();
		vectorList.push_back(vector3(.3f, 0.8f, .3f));
		vectorList.push_back(vector3(-.3f, 0.0f, -.12f));
		m_pLegsRB = new MyRigidBody(vectorList);

		vectorList.clear();
		vectorList.push_back(vector3(-.22f, 1.52f, .82f));
		vectorList.push_back(vector3(-.52f, 1.2f, -.02f));
		m_pLArmRB = new MyRigidBody(vectorList);

		vectorList.clear();
		vectorList.push_back(vector3(.22f, 1.52f, .82f));
		vectorList.push_back(vector3(.52f, 1.2f, -.02f));
		m_pRArmRB = new MyRigidBody(vectorList);

		m_bInMemory = true; //mark this entity as viable
	}
}

void Simplex::Zombie::Initialize(vector3 position)
{
	m_m4ToWorld = glm::rotate(glm::translate(m_m4ToWorld, position),
		static_cast <float> (std::rand() / (static_cast<float>(RAND_MAX / 360))),
		vector3(-1 + static_cast<float>(std::rand() / (static_cast<float>(RAND_MAX / (1 - -1)))),
			-1 + static_cast<float>(std::rand() / (static_cast<float>(RAND_MAX / (1 - -1)))),
			-1 + static_cast<float>(std::rand() / (static_cast<float>(RAND_MAX / (1 - -1))))));

	SetVelocity(-.01f + static_cast<float>(std::rand() / (static_cast<float>(RAND_MAX / (.01f - -.01f)))),
		0.0f,
		0.03f + static_cast<float>(std::rand() / (static_cast<float>(RAND_MAX / 0.02f))));
	SetPosition(position);
	SetModelMatrix(m_m4ToWorld);
}

void Simplex::Zombie::Resolve(BouncyBall * other)
{
	if (!m_bInMemory || !other->GetActive()) return;

	if (other->GetRigidBody()->IsColliding(m_pHeadRB)) {
		m_bInMemory = false;
		other->SetVelocity(-other->GetVelocity());
	}
	else if (other->GetRigidBody()->IsColliding(m_pTorsoRB) && !m_pTorsoRB->isHit) {
		m_pTorsoRB->isHit = true;
		other->SetVelocity(-other->GetVelocity());
	}
	else if (other->GetRigidBody()->IsColliding(m_pLegsRB) && !m_pLegsRB->isHit) {
		m_pLegsRB->isHit = true;
		other->SetVelocity(-other->GetVelocity());
	}
	else if (other->GetRigidBody()->IsColliding(m_pLArmRB) && !m_pLArmRB->isHit) {
		m_pLArmRB->isHit = true;
		other->SetVelocity(-other->GetVelocity());
	}
	else if (other->GetRigidBody()->IsColliding(m_pRArmRB) && !m_pRArmRB->isHit) {
		m_pRArmRB->isHit = true;
		other->SetVelocity(-other->GetVelocity());
	}
}

void Simplex::Zombie::Resolve(Wall * other)
{
	if (!m_bInMemory || !other->GetActive()) return;

	position -= velocity;
	SetVelocity(velocity - 2 * glm::dot(velocity, other->GetForward()) * other->GetForward());
}

void Simplex::Zombie::Resolve(Zombie * other)
{
	if (!m_bInMemory || !other->GetActive()) return;

	vector3 desiredVel = glm::normalize(position - other->GetPosition()) * .002f;

	//acceleration += desiredVel - velocity;
	//other->acceleration += -acceleration;
}

void Simplex::Zombie::Update()
{
	if (!m_bInMemory) return;

	vector3 playerPos = vector3(0.0f, 0.0f, 10.0f);

	if (position.z > -5.0f)
	{
		//velocity = glm::normalize((playerPos - position)) * 0.02f;
		acceleration += glm::normalize((playerPos - position)) * 0.03f - velocity;
	}

	if (m_pHeadRB != nullptr) 
		m_pHeadRB->AddToRenderList();

	if (m_pTorsoRB != nullptr)
		m_pTorsoRB->AddToRenderList(true);

	if (m_pLegsRB != nullptr)
		m_pLegsRB->AddToRenderList(true);

	if (m_pLArmRB != nullptr)
		m_pLArmRB->AddToRenderList(true);

	if (m_pRArmRB != nullptr)
		m_pRArmRB->AddToRenderList(true);


	MyEntity::Update();
	SetRBMatrix(m_m4ToWorld);
}

void Simplex::Zombie::SetRBMatrix(matrix4 a_m4ToWorld)
{
	if (!m_bInMemory) return;

	if (m_pHeadRB != nullptr)
		m_pHeadRB->SetModelMatrix(a_m4ToWorld);

	if (m_pTorsoRB != nullptr)
		m_pTorsoRB->SetModelMatrix(a_m4ToWorld);

	if (m_pLegsRB != nullptr)
		m_pLegsRB->SetModelMatrix(a_m4ToWorld);

	if (m_pLArmRB != nullptr)
		m_pLArmRB->SetModelMatrix(a_m4ToWorld);

	if (m_pRArmRB != nullptr)
		m_pRArmRB->SetModelMatrix(a_m4ToWorld);

}
