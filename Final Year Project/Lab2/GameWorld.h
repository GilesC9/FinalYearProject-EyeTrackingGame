#include "stdafx.h"
#ifndef _GAME_WORLD_INC_
#define _GAME_WORLD_INC_

#include "Terrain.h"
#include "Skybox.h"
#include "SkyDome.h"
#include "TestDynamicObject.h"
#include "Enemy.h"
#include "Cannonball.h"

class GameWorld
{
private:
	osg::ref_ptr<osg::Group> m_pSceneGraphRoot;
	Terrain *m_pTerrain;
	Terrain *m_pTerrain2;
	SkyDome *m_pSkyDome;
	Enemy *m_pDragon;
	Cannonball *m_pCannonball;

	double m_dt;
	double m_gravity;
public:
	std::vector<Cannonball*> ballList;
	std::vector<Enemy*> enemyList;
	int enemiesLeft = 0;
	int shotsFired = 0;
	int hits = 0;
	GameWorld()
	{
		m_dt = 0.01;
		m_gravity = -9.81;
		ballList = std::vector<Cannonball*>();
		enemyList = std::vector<Enemy*>();
	}
	~GameWorld(){}

	void CreateLevel();

	osg::Group *GetSceneGraph()
	{
		return m_pSceneGraphRoot;
	}

	Terrain *GetTerrain()
	{
		return m_pTerrain;
	}

	void Update();

	double GetDT() { return m_dt; }
	double GetGravity() { return m_gravity; }
	Enemy *GetDragonPointer() { return m_pDragon; }
	Cannonball *GetCannonballPointer() { return m_pCannonball; }
	void SetCannonballPointer(Cannonball * cb) { m_pCannonball = cb; }
};
#endif
