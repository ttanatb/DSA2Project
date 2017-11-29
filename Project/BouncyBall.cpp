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
	}
}

void Simplex::BouncyBall::Initialize(vector3 position, vector3 forward)
{
	m_m4ToWorld = glm::rotate(glm::translate(m_m4ToWorld, position), 45.0f, vector3(1));
	SetModelMatrix(m_m4ToWorld);
}

void Simplex::BouncyBall::Resolve(BouncyBall * other)
{
	//vector3 thisPos, otherPos, thisVel, otherVel;

	//vector3 xDir = glm::normalize(thisPos - otherPos);

	//vector3 v1 = thisVel;
	//float x1 = glm::dot(v1, AXIS_X);
}

void Simplex::BouncyBall::Resolve(Wall * other)
{
}
