#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Tim Yuan - txy9233@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));
	
	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUp(AXIS_Z * 20.0f, ZERO_V3, AXIS_Y);

	//if the light position is zero move it
	if (m_pLightMngr->GetPosition(1) == ZERO_V3)
		m_pLightMngr->SetPosition(vector3(0.0f, 0.0f, 3.0f));

	//if the background is cornflowerblue change it to black (its easier to see)
	if (vector3(m_v4ClearColor) == C_BLUE_CORNFLOWER)
	{
		m_v4ClearColor = vector4(ZERO_V3, 1.0f);
	}
	
	//if there are no segments create 7
	if(m_uOrbits < 1)
		m_uOrbits = 7;

	float fSize = 1.0f; //initial size of orbits

	//creating a color using the spectrum 
	uint uColor = 650; //650 is Red
	//prevent division by 0
	float decrements = 250.0f / (m_uOrbits > 1? static_cast<float>(m_uOrbits - 1) : 1.0f); //decrement until you get to 400 (which is violet)
	/*
		This part will create the orbits, it start at 3 because that is the minimum subdivisions a torus can have
	*/
	uint uSides = 3; //start with the minimal 3 sides
	for (uint i = uSides; i < m_uOrbits + uSides; i++)
	{
		vector3 v3Color = WaveLengthToRGB(uColor); //calculate color based on wavelength
		m_shapeList.push_back(m_pMeshMngr->GenerateTorus(fSize, fSize - 0.1f, 3, i, v3Color)); //generate a custom torus and add it to the meshmanager
		
		// calculate the positions for each orbit
		// the theory: 
		// 1. each orbit has a common start position - vec3(.95 + (.5f * i), 0, 0) where i is the orbit # (1 = 3 sided, 2 = 4 sided, etc.)
		// 2. the x value of the vec3 = same distance for all points along that path from origin (radius) = fsize - .05
		// 3. the angle separatig each point if they lay on a circle would be i * 360/uSides
		// 4. therefore, any given point would be radius * cos(angle), radius * sin(angle), 0
		// 5. so for the triangle, the points would be
		//		.95f * cos(0), .95f * sin(0), 0 = .95f, 0, 0
		//		.95f * cos(60), .95f * sin(60)... you get the point. literally.


		// temporary vector that will hold positions for the stops of each circuit
		std::vector<vector3>stops;
		vector3 stop;
		float radius = fSize - 0.05f;
		float angle = 0.0f;
		for (uint j = 0; j < i; ++j) {
			stop = vector3(
				//calculations for positions
				radius * glm::cos(glm::radians(angle)),
				radius * glm::sin(glm::radians(angle)),
				0);
			stops.push_back(stop);
			angle += (360 / i);
		}

		// add that list of stops to the master list
		m_stopsList.push_back(stops);
		stopsCounter.push_back(0);
		orbitStops.push_back(i);
		fSize += 0.5f; //increment the size for the next orbit
		uColor -= static_cast<uint>(decrements); //decrease the wavelength

	
	}

	

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

	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix
	matrix4 m4Offset = IDENTITY_M4; //offset of the orbits, starts as the global coordinate system
	/*
		The following offset will orient the orbits as in the demo, start without it to make your life easier.
	*/
	m4Offset = glm::rotate(IDENTITY_M4, 90.0f, AXIS_Z);
	static float fTime = 0.0f;
	static uint uClock = m_pSystem->GenClock();
	float fDelta = m_pSystem->GetDeltaTime(uClock);
	fTime += fDelta;
	// draw a shapes
	for (uint i = 0; i < m_uOrbits; ++i)
	{
		m_pMeshMngr->AddMeshToRenderList(m_shapeList[i], glm::rotate(m4Offset, 90.0f, AXIS_X));

		//calculate the current position
		vector3 v3CurrentPos = ZERO_V3;
		
		// my code here
		if (stopsCounter[i] < m_stopsList[i].size() - 1)
			v3CurrentPos = glm::lerp(m_stopsList[i][stopsCounter[i]], m_stopsList[i][stopsCounter[i] + 1], fTime);
		else
			v3CurrentPos = glm::lerp(m_stopsList[i][m_stopsList[i].size() - 1], m_stopsList[i][0], fTime);
		
		matrix4 m4Model = glm::translate(m4Offset, v3CurrentPos);
		
		//draw spheres
		m_pMeshMngr->AddSphereToRenderList(m4Model * glm::scale(vector3(0.1)), C_WHITE);	
		
	
	}
		
	//increment the counter for each orbit


	if (fTime >= 1.0f)
	{
		fTime = 0.0f;	

		for (uint i = 0; i < m_uOrbits; ++i)
		{
			++stopsCounter[i];
			// this should make it so that it loops back around to 0 each time?
			stopsCounter[i] %= orbitStops[i];
		}
	}
	

	

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
	//release GUI
	ShutdownGUI();
}