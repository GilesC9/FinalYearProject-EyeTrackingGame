#ifndef _GAME_
#define _GAME_

#include "stdafx.h"
#include "Terrain.h"
#include "GameWorld.h"
#include "InputHandler.h"
#include "EyeTracker.h"

class Game
{
private:
	InputHandler *pInputHandler;
	osgViewer::Viewer viewer;

	GameWorld *g_pGameWorld;
	EyeTracker* m_pTracker;


	//Camera Input
	osgGA::TrackballManipulator *m_trackBall;
	float terrainOffset, heading, pitch;
	osg::Vec3 camPos;
	osg::Vec3 camDir;
	osg::Vec3 upDir;
	osg::Vec3 sideDir;
	osg::Vec3 moveDir;
	float initX = 0, initY = 0;

public:
	Game();
	~Game(){};

	//void Update();
	void Update(cv::Point TrackingData, cv::Point initDist); //Args will be replaced by appropriate type for tracking data!
	//void HandleInput();
	void HandleInput(osg::Vec3 EyeCentre, osg::Vec3 initDist);
	void Start(EyeTracker* tracker);

	osgViewer::Viewer GetViewer() { return viewer; }

	//Helpers
	osg::Vec3 RotateVectorAboutAxis(osg::Vec3 v, osg::Vec3 axis, float angle_deg);
};

#endif