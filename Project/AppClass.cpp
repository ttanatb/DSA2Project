#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Tanat Boozayaangool - txb9038@rit.edu";

	m_pRandom = Random::GetInstancce();

	//Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUp(
		vector3(0.0f, 1.8f, 10.0f), //Position
		vector3(0.0f, 1.8f,  9.0f),	//Target
		AXIS_Y);					//Up

	m_pCamera = m_pCameraMngr->GetCamera();

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

	//Entity Manager
	m_pEntityMngr = MyEntityManager::GetInstance();

	//creeper
	for (uint i = 0; i < 200; ++i)
		m_pEntityMngr->AddZombie(m_pRandom->Next(vector3(-8.0f, .0f, -25.0f), vector3(8.0f, 0.0f, -200.0f)));

	m_pEntityMngr->AddWall(vector3(-10.0f, -2.0f, 0.0f), true);
	m_pEntityMngr->AddWall(vector3(10.0f, -2.0f, 0.0f), true);
	m_pEntityMngr->AddFloor();

	m_pRootOctant = new MyOctant(m_uOctantMaxLevel, m_uOctantIdealCount);

	m_pPlayer = new Player();
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	m_pPlayer->Update(m_pCameraMngr->GetPosition(), m_pCameraMngr->GetForward(), m_qFPC);

	if (!m_bLeftWasClicked && m_bLeftIsClicked) {
		m_pEntityMngr->AddBall(m_pCameraMngr->GetPosition(), m_pCameraMngr->GetForward(), 0.5f);
	}

	//m_pEntityMngr->SetTestModelMatrix(glm::translate(m_v3TestPos));
	m_pEntityMngr->SetTestZombieModelMatrix(ToMatrix4(m_qArcBall));

	//Root Octant
	m_pEntityMngr->ClearDimensionSetAll();
	SafeDelete(m_pRootOctant);
	m_pRootOctant = new MyOctant(m_uOctantMaxLevel, m_uOctantIdealCount);

	//Update Entity Manager
	m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddAllToRenderList(true);

	m_bLeftWasClicked = m_bLeftIsClicked;
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	//render octant
	if (m_bIsDebug)
	{
		m_pRootOctant->Display(-1);
	}

	m_pPlayer->Render();

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//draw gui
	DrawGUI();

	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release the entity manager
	m_pEntityMngr->ReleaseInstance();

	SafeDelete(m_pPlayer);

	//release GUI
	ShutdownGUI();
}