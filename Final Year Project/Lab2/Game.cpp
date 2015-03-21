#include "Game.h"

Game::Game()
{

}

void Game::Start(EyeTracker* tracker)
{
	//while (!viewer.done())
	//{
	//	Update();
	//}
	m_pTracker = tracker;

	// create the game world
	g_pGameWorld = new GameWorld;
	g_pGameWorld->CreateLevel();

	//Create new input event handler to viwer class
	pInputHandler = new InputHandler();
	viewer.addEventHandler(pInputHandler);

	//Configure the view to render in a window
	viewer.apply(new osgViewer::SingleWindow(50, 50, 1280, 720));

	//Create the game world, and 
	viewer.setSceneData(g_pGameWorld->GetSceneGraph());

	//Change the dafault lighting set up to a sky light (it defaults to 'headlight')
	viewer.getCamera()->getView()->setLightingMode(osg::View::SKY_LIGHT);

	//Init camera position
	terrainOffset = 5.0f;
	pitch = 0.0f;
	heading = 0.0f;
	camDir = osg::Vec3(1.0f, 0.0f, 0.0f);
	upDir = osg::Vec3(0.0f, 0.0f, 1.0f);
	sideDir = osg::Vec3(0.0f, 1.0f, 0.0f);

	camPos[0] = 0.5f * (g_pGameWorld->GetTerrain()->GetX0() + g_pGameWorld->GetTerrain()->GetX1());
	camPos[1] = 0.5f * (g_pGameWorld->GetTerrain()->GetY0() + g_pGameWorld->GetTerrain()->GetY1());
	camPos[2] = g_pGameWorld->GetTerrain()->HeightAtPoint(camPos[0], camPos[1]) + terrainOffset; //Set above terrain at point

	viewer.getCamera()->setViewMatrixAsLookAt(camPos, camPos + camDir, upDir);
}

//void Game::Update()
//{
//	HandleInput();
//	g_pGameWorld->Update();
//	viewer.frame(); //Update frame
//}

void Game::Update(cv::Point TrackingData, cv::Point initDist) //Args will be replaced by appropriate type for tracking data!
{
	osg::Vec3 eyeCentre = osg::Vec3(TrackingData.x, TrackingData.y, 0);
	osg::Vec3 eyeInitCentre = osg::Vec3(initDist.x, initDist.y, 0);
	HandleInput(eyeCentre, eyeInitCentre);

	
	g_pGameWorld->Update();
	viewer.frame(); //Update frame
}

void Game::HandleInput(osg::Vec3 EyeCentre, osg::Vec3 initEyeCentre)
{
	//Polling the control system
	float x, y, dx, dy;
	int bMouse, bDirections, bButtons;
	pInputHandler->GetUpdate(dx, dy, x, y, bMouse, bDirections, bButtons);

	//Keyboard control
	//Moving the camera
	if (bDirections & InputHandler::FORWARD)
	{
		//Update camPos to move forward	
		moveDir = camDir;
		moveDir[2] = 0.0f;
		moveDir.normalize();

		camPos[0] += moveDir.x();
		camPos[1] += moveDir.y();
		camPos[2] = g_pGameWorld->GetTerrain()->HeightAtPoint(camPos[0], camPos[1]) + terrainOffset; //Set above terrain at point			
	}
	if (bDirections & InputHandler::BACK)
	{
		//Update camPos to move back
		//moveDir = camDir;
		//moveDir[2] = 0.0f;
		//moveDir.normalize();

		//camPos[0] -= moveDir.x();
		//camPos[1] -= moveDir.y();
		//camPos[2] = g_pGameWorld->GetTerrain()->HeightAtPoint(camPos[0], camPos[1]) + terrainOffset; //Set above terrain at point
		
		float fDeltaY = EyeCentre.y() - initEyeCentre.y();
		float fAbsY = EyeCentre.y();

		if (fDeltaY < -3 || fDeltaY > 3) //Looking up/down
		{
			pitch += fDeltaY * 0.25f;

			if (pitch > 70)
			{
				pitch = 70;
			}

			if (pitch < -70)
			{
				pitch = -70;
			}
		}
	}
	if (bDirections & InputHandler::LEFT)
	{
		//if (heading < 0.0f)
		//	heading += 360.0f;
		//if (heading > 360.0f)
		//	heading -= 360.0f;

		////Update heading
		//heading += 1.0f;
		
		float fDeltaX = EyeCentre.x() - initEyeCentre.x();
		float fAbsX = EyeCentre.x();

		//if (fDeltaX > 5)
		//	initX = EyeCentre.x();
		//if (fDeltaY > 5)
		//	initY = EyeCentre.y();

		//moveDir = EyeCentre;
		//moveDir[2] = 0.0f;
		//moveDir.normalize();

		if (fDeltaX < -3 || fDeltaX > 3) //Looking left/right
		{
			//camPos[0] += sideDir.x();//0;// fDeltaX;
			//camPos[1] += sideDir.y();//-fDeltaX * 3;
			//camPos[2] = g_pGameWorld->GetTerrain()->HeightAtPoint(camPos[0], camPos[1]) + terrainOffset; //Set above terrain at point 
			
			heading -= fDeltaX * 0.25f;

			if (heading < 0.0f)
				heading += 360.0f;
			if (heading > 360.0f)
				heading -= 360.0f;
		}
	}

	if (bDirections & InputHandler::RIGHT)
	{
		//if (heading < 0.0f)
		//	heading += 360.0f;
		//if (heading > 360.0f)
		//	heading -= 360.0f;

		////Update heading
		//heading -= 1.0f;

		//moveDir = sideDir;
		//moveDir[2] = 0.0f;
		//moveDir.normalize();

		//camPos[0] -= sideDir.x();
		//camPos[1] -= sideDir.y();
		//camPos[2] = g_pGameWorld->GetTerrain()->HeightAtPoint(camPos[0], camPos[1]) + terrainOffset; //Set above terrain at point

		m_pTracker->Calibrate(); //Reset inti eye centre
	}

	//Mouse control
	if (bMouse)
	{
		//Update heading based on dx, pitch based on dy.	
		//Update heading
		heading += -dx;// * 0.5f;

		if (heading < 0.0f)
			heading += 360.0f;
		if (heading > 360.0f)
			heading -= 360.0f;

		pitch += -dy;// * 0.5f;

		if (pitch > 70)
		{
			pitch = 70;
		}

		if (pitch < -70)
		{
			pitch = -70;
		}
	}

	//Misc Buttons
	//if (bButtons & InputHandler::TRACKBALL)
	//{
	//	m_trackBall = new osgGA::TrackballManipulator();
	//	viewer.setCameraManipulator(m_trackBall);
	//}

	//if (bButtons & InputHandler::FIRSTPERSON)
	//{
	//	viewer.setCameraManipulator(NULL); //Switch off master camera manipulator

	//	//Init camera position
	//	terrainOffset = 5.0f;
	//	pitch = 0.0f;
	//	heading = 0.0f;
	//	camDir = osg::Vec3(1.0f, 0.0f, 0.0f);
	//	upDir = osg::Vec3(0.0f, 0.0f, 1.0f);
	//	sideDir = osg::Vec3(0.0f, 1.0f, 0.0f);

	//	camPos[0] = 0.5f * (g_pGameWorld->GetTerrain()->GetX0() + g_pGameWorld->GetTerrain()->GetX1());
	//	camPos[1] = 0.5f * (g_pGameWorld->GetTerrain()->GetY0() + g_pGameWorld->GetTerrain()->GetY1());
	//	camPos[2] = g_pGameWorld->GetTerrain()->HeightAtPoint(camPos[0], camPos[1]) + terrainOffset; //Set above terrain at point

	//	viewer.getCamera()->setViewMatrixAsLookAt(camPos, camPos + camDir, upDir);
	//}

	//Cannonball
	if (bButtons & InputHandler::FIRE)
	{
		Cannonball *ball = new Cannonball(g_pGameWorld, camPos[0], camPos[1], camPos[2]);
		g_pGameWorld->ballList.push_back(ball);
		ball->Fire(camDir);		
	}
	if (bButtons & InputHandler::RESET)
	{
		g_pGameWorld->GetCannonballPointer()->Reset();
		g_pGameWorld->ballList.clear();
	}

	camDir = osg::Vec3(1.0f, 0.0f, 0.0f);
	upDir = osg::Vec3(0.0f, 0.0f, 1.0f);
	sideDir = osg::Vec3(0.0f, 1.0f, 0.0f);
	EyeCentre = initEyeCentre;

	camDir = RotateVectorAboutAxis(camDir, upDir, heading);
	sideDir = RotateVectorAboutAxis(sideDir, upDir, heading);

	EyeCentre = RotateVectorAboutAxis(EyeCentre, upDir, heading);

	camDir = RotateVectorAboutAxis(camDir, sideDir, pitch);
	upDir = RotateVectorAboutAxis(upDir, sideDir, pitch);

	viewer.getCamera()->setViewMatrixAsLookAt(camPos, camPos + camDir, upDir);
}

//Helper function for the camera calculations
osg::Vec3 Game::RotateVectorAboutAxis(osg::Vec3 v, osg::Vec3 axis, float angle_deg)
{
	float ct, st;
	ct = cos(angle_deg * 0.017453293f);
	st = sin(angle_deg * 0.017453293f);

	osg::Vec3 answer;
	answer = v * ct + (axis ^ v) * st + axis * (axis * v) * (1.0f - ct);

	return answer;
}