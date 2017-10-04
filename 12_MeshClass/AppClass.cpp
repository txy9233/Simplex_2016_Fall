#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	//m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Make MyMesh object
	m_pMesh = new MyMesh[3];
	m_pMesh[0].GenerateCube(1.0f, C_BROWN);
	m_pMesh[1].GenerateCube(1.0f, C_BROWN);
	m_pMesh[2].GenerateCube(1.0f, C_BROWN);

	////Make MyMesh object
	//m_pMesh1 = new MyMesh();
	//m_pMesh1->GenerateCube(1.0f, C_WHITE);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	vector3 v3Position(1,0, 0);
	matrix4 m4Model; //defaults to identity matrix when no assign
	m4Model = glm::translate(IDENTITY_M4, v3Position);
	
	//never gets memory reallocated, so constant growth/movement
	/*static matrix4 m4Model; 
	m4Model = glm::translate/scale/rotate(m4Model, v3Position);*/
	
	
	//m4Model[3] = vector4(v3Position, 1); //this is 4x4, must convert
	//m4Model[1][1] = 0.5f; //squish down
	//m4Model[2][2] = 0.5f;
	//m4Model[3][3] = 0.5f;
	

	m_pMesh->Render(m4Projection, m4View, m4Model);
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
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
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	//SafeDelete(m_pMesh1);
	delete[] m_pMesh;
	//release GUI
	ShutdownGUI();
}