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
		m_pRigidBody = new MyRigidBody(m_pModel->GetVertexList()); //generate a rigid body
		m_bInMemory = true; //mark this entity as viable
	}
}

void Simplex::BouncyBall::Initialize(vector3 position, vector3 forward)
{
	m_m4ToWorld = glm::translate(m_m4ToWorld, position);
	SetModelMatrix(m_m4ToWorld);
}