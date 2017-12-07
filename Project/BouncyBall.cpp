#include "BouncyBall.h"
using namespace Simplex;

Simplex::BouncyBall::BouncyBall()
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
		m_pRigidBody = new MyRigidBody(m_pModel->GetVertexList(), true); //generate a rigid body
		m_bInMemory = true; //mark this entity as viable
		radius = m_pRigidBody->GetRadius();
	}
}

void Simplex::BouncyBall::Initialize(vector3 position, vector3 forward, float speed)
{
    SetPosition(position);
    SetForward(forward);
    SetVelocity(forward * speed);
    /*
    m_m4ToWorld = glm::rotate(glm::translate(m_m4ToWorld, position), 45.0f, vector3(1));
    SetModelMatrix(m_m4ToWorld);
    */
}

void Simplex::BouncyBall::Resolve(BouncyBall * other)
{
	if (m_bInMemory && other->m_bInMemory) {
		SetActive(false);
		other->SetActive(false);
	}
}

void Simplex::BouncyBall::Resolve(Wall * other)
{
	if (m_bInMemory && other->GetActive()) {
		SetVelocity(0.95f * (velocity - 2 * glm::dot(velocity, other->GetForward()) * other->GetForward()));
	}
}

void Simplex::BouncyBall::Update() 
{
	acceleration = AXIS_Y * -0.005f;

	MyEntity::Update();

	if (position.y < FLOOR_POS + radius) {
		velocity.y = -velocity.y * 0.95f;
		position.y = FLOOR_POS + radius;
	}
}
