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
	SetPosition(position);
	SetModelMatrix(m_m4ToWorld);
}

void Simplex::Zombie::Resolve(BouncyBall * other)
{
}

void Simplex::Zombie::Resolve(Wall * other)
{
}

void Simplex::Zombie::Resolve(Zombie * other)
{
}
