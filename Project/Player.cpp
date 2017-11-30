#include "Player.h"
using namespace Simplex;

Simplex::Player::Player()
{
	m_pCrosshair = new MyEntity(CROSSHAIR_FILE_PATH);
}

Simplex::Player::~Player()
{
}

void Simplex::Player::Update(vector3 camPos, vector3 camForward)
{
	//std::cout << "Forward: " << camForward.x << ", " << camForward.y << ", " << camForward.z << std::endl;
	m_pCrosshair->SetPosition(camPos + camForward);

	if (glm::dot(camForward, AXIS_Z) < 0) {
		camForward = -camForward;
	}

	m_pCrosshair->SetForward(camForward);
	m_pCrosshair->Update();
}

void Simplex::Player::Render()
{
	if (m_pCrosshair != nullptr)
		m_pCrosshair->AddToRenderList();
}
