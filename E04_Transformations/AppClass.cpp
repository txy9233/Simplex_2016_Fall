#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	//m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Make MyMesh object
	m_pMesh = new MyMesh[46];
	for (uint i = 0; i < 46; ++i)
	{
		m_pMesh[i].GenerateCube(1.0f, C_BLACK);
	}
	positionsArr = new vector3[46];
	positionsArr[0] = vector3(4, 1, 0);
	positionsArr[1] = vector3(5, 1, 0);
	positionsArr[2] = vector3(7, 1, 0);
	positionsArr[3] = vector3(8, 1, 0);

	positionsArr[4] = vector3(1, 2, 0);
	positionsArr[5] = vector3(3, 2, 0);
	positionsArr[6] = vector3(9, 2, 0);
	positionsArr[7] = vector3(11, 2, 0);

	positionsArr[8] = vector3(1, 3, 0);
	positionsArr[9] = vector3(3, 3, 0);
	positionsArr[10] = vector3(4, 3, 0);
	positionsArr[11] = vector3(5, 3, 0);
	positionsArr[12] = vector3(6, 3, 0);
	positionsArr[13] = vector3(7, 3, 0);
	positionsArr[14] = vector3(8, 3, 0);
	positionsArr[15] = vector3(9, 3, 0);
	positionsArr[16] = vector3(11, 3, 0);
	
	positionsArr[17] = vector3(1, 4, 0);
	positionsArr[18] = vector3(2, 4, 0);
	positionsArr[19] = vector3(3, 4, 0);
	positionsArr[20] = vector3(4, 4, 0);
	positionsArr[21] = vector3(5, 4, 0);
	positionsArr[22] = vector3(6, 4, 0);
	positionsArr[23] = vector3(7, 4, 0);
	positionsArr[24] = vector3(8, 4, 0);
	positionsArr[25] = vector3(9, 4, 0);
	positionsArr[26] = vector3(10, 4, 0);
	positionsArr[27] = vector3(11, 4, 0);
	
	positionsArr[28] = vector3(2, 5, 0);
	positionsArr[29] = vector3(3, 5, 0);
	positionsArr[30] = vector3(5, 5, 0);
	positionsArr[31] = vector3(6, 5, 0);
	positionsArr[32] = vector3(7, 5, 0);
	positionsArr[33] = vector3(9, 5, 0);
	positionsArr[34] = vector3(10, 5, 0);
	
	positionsArr[35] = vector3(3, 6, 0);
	positionsArr[36] = vector3(4, 6, 0);
	positionsArr[37] = vector3(5, 6, 0);
	positionsArr[38] = vector3(6, 6, 0);
	positionsArr[39] = vector3(7, 6, 0);
	positionsArr[40] = vector3(8, 6, 0);
	positionsArr[41] = vector3(9, 6, 0);
	
	positionsArr[42] = vector3(4, 7, 0);
	positionsArr[43] = vector3(8, 7, 0);
	
	positionsArr[44] = vector3(3, 8, 0);
	positionsArr[45] = vector3(9, 8, 0);
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
	vector3 v3Position(1, 2, 1);
	static matrix4 m4Model; //defaults to identity matrix when no assign
	
	//motion
	m4Model = glm::translate(m4Model, vector3(glm::cos(xlatr), glm::sin(xlatr), 0.0f));
	
	//never gets memory reallocated, so constant growth/movement
	/*static matrix4 m4Model; 
	m4Model = glm::translate/scale/rotate(m4Model, v3Position);*/

	//initial positions
	for (int i = 0; i < 46; ++i)
	{
		m_pMesh[i].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, positionsArr[i]));
	}
	
	
	//m4Model[3] = vector4(v3Position, 1); //this is 4x4, must convert
	//m4Model[1][1] = 0.5f; //squish down
	//m4Model[2][2] = 0.5f;
	//m4Model[3][3] = 0.5f;
	

	//m_pMesh->Render(m4Projection, m4View, m4Model);
	
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

	xlatr += 0.15f;
}
void Application::Release(void)
{
	/*if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}*/
	//SafeDelete(m_pMesh1);
	delete[] m_pMesh;
	delete[] positionsArr;
	//release GUI
	ShutdownGUI();
}