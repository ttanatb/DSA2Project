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

	m_bInMemory = false;
	other->SetActive(false);
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

	acceleration += desiredVel - velocity;
	other->acceleration += -acceleration;
}

void Simplex::Zombie::Update()
{
	vector3 playerPos = vector3(0.0f, 0.0f, 10.0f);

	if (!m_pRigidBody->IsCollidingWithSomething())
	{
		//velocity = glm::normalize((playerPos - position)) * 0.02f;
		acceleration += glm::normalize((playerPos - position)) * 0.02f - velocity;
	}
	//else std::cout << "ASDFADSF";
	MyEntity::Update();
}
