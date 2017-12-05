#include "Wall.h"
using namespace Simplex;

Simplex::Wall::Wall()
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

void Simplex::Wall::Initialize(vector3 position, vector3 forward, vector3 scale)
{
	SetPosition(position);

	quaternion rot = IDENTITY_QUAT;
	if (forward == AXIS_Z || forward == -AXIS_Z) {
		rot = glm::angleAxis(90.0f, AXIS_Y);
	}

	rotation = rot;
	
	SetModelMatrix(glm::translate(position) * ToMatrix4(rotation));
	SetForward(forward);
}