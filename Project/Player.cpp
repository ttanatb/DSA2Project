#include "Player.h"
using namespace Simplex;

Simplex::Player::Player()
{
	m_pCrosshair = new MyEntity(CROSSHAIR_FILE_PATH);
}

Simplex::Player::~Player()
{
}

void Simplex::Player::Update(vector3 camPos, vector3 camForward, quaternion q_CamRot)
{
	//m_pCrosshair->SetPosition(camPos + camForward);
	//m_pCrosshair->SetRotation(q_CamRot);
	m_pCrosshair->SetModelMatrix(glm::translate(IDENTITY_M4, camPos) * ToMatrix4(q_CamRot));
	//m_pCrosshair->Update();
}

void Simplex::Player::Render()
{
	if (m_pCrosshair != nullptr)
		m_pCrosshair->AddToRenderList();
}
