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
	//vector3 thisPos, otherPos, thisVel, otherVel;

	//vector3 xDir = glm::normalize(this->position - other->position);

	//vector3 v1 = thisVel;
	//float x1 = glm::dot(this->velocity, AXIS_X);
}

void Simplex::BouncyBall::Resolve(Wall * other)
{
}

void Simplex::BouncyBall::Update() 
{
	if (position.y < FLOOR_POS + radius) {
		velocity.y = -velocity.y;
		position.y = FLOOR_POS + radius;
	}

	MyEntity::Update();
}
