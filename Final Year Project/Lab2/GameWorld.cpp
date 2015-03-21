#include "stdafx.h"
#include "GameWorld.h"
#include "EyeTracker.h"

#define GRIDSIZE 2

void GameWorld::CreateLevel(void)
{
	m_pSceneGraphRoot = new osg::Group;

	//Create static objects
	m_pTerrain = new Terrain();
    //m_pTerrain->CreateFromBitmap( "Heightmaps\\mountains512.bmp", 50.0f, 1.0f );
	m_pTerrain->CreateFromBitmap("Heightmaps\\mountains2.bmp", 90.0f, 1.0f);	

#pragma region Create skybox
	//Skybox
	//osg::ref_ptr<osg::Geode> pSkyBoxNode = new osg::Geode;
	//pSkyBoxNode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(), m_pTerrain->GetDelta() * 10)));
	//osg::ref_ptr<osg::Image> posx = osgDB::readImageFile("Textures\\Skybox\\posx.jpg");
	//osg::ref_ptr<osg::Image> negx = osgDB::readImageFile("Textures\\Skybox\\negx.jpg");
	//osg::ref_ptr<osg::Image> posy = osgDB::readImageFile("Textures\\Skybox\\posy.jpg");
	//osg::ref_ptr<osg::Image> negy = osgDB::readImageFile("Textures\\Skybox\\negy.jpg");
	//osg::ref_ptr<osg::Image> posz = osgDB::readImageFile("Textures\\Skybox\\posz.jpg");
	//osg::ref_ptr<osg::Image> negz = osgDB::readImageFile("Textures\\Skybox\\negz.jpg");

	//osg::ref_ptr<SkyBox> pSkyBox = new SkyBox;
	//pSkyBox->getOrCreateStateSet()->setTextureAttributeAndModes(0, new osg::TexGen);
	//pSkyBox->setEnvironmentMap(0, posx, negx, posz, negz, posy, negy);
	//pSkyBox->addChild(pSkyBoxNode);

	//Skydome
	m_pSkyDome = new SkyDome();
	osg::ref_ptr<osg::Geode> pSkyBoxNode = new osg::Geode;
	m_pSkyDome->CreateSkyDome(pSkyBoxNode, 400, 30, 30);

	//Add transform for skydome
	osg::ref_ptr<osg::PositionAttitudeTransform> skyDomeTrans = new osg::PositionAttitudeTransform();

	//Set position
	skyDomeTrans->setPosition(osg::Vec3(0, 0, -100));
	skyDomeTrans->addChild(pSkyBoxNode);
#pragma endregion

	//Create the geometry node
	osg::ref_ptr<osg::Geode> pGeode = new osg::Geode;
	m_pTerrain->CreateOSGGeode(pGeode); //Create terrain geomtry

	//m_pSceneGraphRoot->addChild(pSkyBox); //Skybox
	m_pSceneGraphRoot->addChild(skyDomeTrans); //Skydome
	m_pSceneGraphRoot->addChild(pGeode);

//#pragma region Read in models
//	//Read in tree model
//	osg::Node *treeNode = osgDB::readNodeFile("Models\\Tree\\untitled.dae");
//	//Create a parent node for all the trees
//	osg::ref_ptr<osg::Group> treeGroup[GRIDSIZE][GRIDSIZE];// = new osg::Group;
//
//	//Read in rock model
//	osg::Node *rockNode = osgDB::readNodeFile("Models\\Rock\\model.dae");
//	//Create a parent node for all the rocks
//	osg::ref_ptr<osg::Group> rockGroup[GRIDSIZE][GRIDSIZE];// = new osg::Group;
//	
//	//Create space in memory for grid nodes
//	for(int i = 0; i < GRIDSIZE; i++)
//	{	
//		for(int j = 0; j < GRIDSIZE; j++)
//		{			
//			treeGroup[i][j] = new osg::Group;
//			rockGroup[i][j] = new osg::Group;
//		}
//	}
//
//#pragma endregion
//
//#pragma region Populate with trees
//	//Loop through adding new instances of trees
//	for (int i = 0; i < 25; i++)
//	{
//		float modelXPos, modelYPos, modelZPos;
//
//		//Random generator between terrain extents
//		std::mt19937 gen(rand());
//		std::uniform_int_distribution<int> disX(m_pTerrain->GetX0(), m_pTerrain->GetX1() - 2);
//		std::uniform_int_distribution<int> disY(m_pTerrain->GetY0(), m_pTerrain->GetY1() - 2);
//
//		//Randomly generate x and y coords within the range of the terrain extents
//		modelXPos = disX(gen);
//		modelYPos = disY(gen);
//		//Get Z from height at XY point
//		modelZPos = m_pTerrain->HeightAtPoint(modelXPos, modelYPos);
//
//		//Get grid cell
//		int xGrid = GRIDSIZE * (modelXPos - m_pTerrain->GetX0()) / (m_pTerrain->GetX1() - m_pTerrain->GetX0());
//		int yGrid = GRIDSIZE * (modelYPos - m_pTerrain->GetY0()) / (m_pTerrain->GetY1() - m_pTerrain->GetY0());
//
//		if (modelZPos < 25.0f) //Do want trees on tops of mountains, that just looks silly!
//		{
//			//Tree
//			//Get angle compared to up vector
//			osg::Vec3 normal = m_pTerrain->NormalAtPoint(modelXPos, modelYPos);
//			osg::Vec3 up = osg::Vec3(0.0, 0.0, 1.0);
//			float angleRad = (normal * up) / (normal.length() * up.length());
//			float angleDeg = osg::RadiansToDegrees(angleRad);
//	
//			//Add an instance of the tree model - first calculate a transform
//			osg::ref_ptr<osg::PositionAttitudeTransform> treeTrans = new osg::PositionAttitudeTransform();
//			//Calculate x, y, z here.
//			treeTrans->setScale(osg::Vec3(0.008, 0.008, 0.008));
//			treeTrans->setAttitude(osg::Quat(angleDeg, normal));
//			treeTrans->setPosition(osg::Vec3(modelXPos, modelYPos, modelZPos));
//			//Add the tree geometry as a child of this transform
//			treeTrans->addChild(treeNode);
//			//Add the transform to the tree group
//			treeGroup[xGrid][yGrid]->addChild(treeTrans);
//			
//		}
//	}
//#pragma endregion
//
//#pragma region Populate with rocks
//	//Loop through adding new instances of rocks
//	for (int i = 0; i < 25; i++)
//	{
//		float modelXPos, modelYPos, modelZPos;
//
//		//Random generator between terrain extents
//		std::mt19937 gen(rand());
//		std::uniform_int_distribution<int> disX(m_pTerrain->GetX0(), m_pTerrain->GetX1() - 2);
//		std::uniform_int_distribution<int> disY(m_pTerrain->GetY0(), m_pTerrain->GetY1() - 2);
//
//		//Randomly generate x and y coords within the range of the terrain extents
//		modelXPos = disX(gen);
//		modelYPos = disY(gen);
//		//Get Z from height at XY point
//		modelZPos = m_pTerrain->HeightAtPoint(modelXPos, modelYPos);
//
//		//Get grid cell
//		int xGrid = GRIDSIZE * (modelXPos - m_pTerrain->GetX0()) / (m_pTerrain->GetX1() - m_pTerrain->GetX0());
//		int yGrid = GRIDSIZE * (modelYPos - m_pTerrain->GetY0()) / (m_pTerrain->GetY1() - m_pTerrain->GetY0());
//
//		if (modelZPos < 30.0f)
//		{
//			//Get angle compared to up vector
//			osg::Vec3 normal = m_pTerrain->NormalAtPoint(modelXPos, modelYPos);
//			osg::Vec3 up = osg::Vec3(0.0, 0.0, 1.0);
//			float angleRad = (normal * up) / (normal.length() * up.length());
//			float angleDeg = osg::RadiansToDegrees(angleRad);
//
//			//Rock
//			//Add an instance of the rock model - first calculate a transform
//			osg::ref_ptr<osg::PositionAttitudeTransform> rockTrans = new osg::PositionAttitudeTransform();
//			//Calculate x, y, z here.
//			rockTrans->setScale(osg::Vec3(0.1, 0.1, 0.1));
//			rockTrans->setAttitude(osg::Quat(angleDeg, normal));
//			rockTrans->setPosition(osg::Vec3(modelXPos, modelYPos, modelZPos));
//			//Add the rock geometry as a child of this transform
//			rockTrans->addChild(rockNode);
//			//Add the transform to the rock group
//			rockGroup[xGrid][yGrid]->addChild(rockTrans);
//		}
//	}
//#pragma endregion
//
//
//	//Add the group to the scene graph
//	for(int i = 0; i < GRIDSIZE; i++)
//	{
//		for(int j = 0; j < GRIDSIZE; j++)
//		{
//			m_pSceneGraphRoot->addChild(rockGroup[i][j]);
//			m_pSceneGraphRoot->addChild(treeGroup[i][j]);
//		}
//	}

	//Create dynamic objects
	//Create a cannonball and a dragon  - KY
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			Enemy* newDragon = new Enemy(this, 0, 0, 0);
			newDragon->SetPosition(Vec3(40, -7 + (i * (newDragon->GetSize() + 5)), 40 + (j * (newDragon->GetSize() + 5))));
			newDragon->GetCollider()->SetRadius(newDragon->GetCollider()->GetRadius() * 1.2); //Increase by 20%
			newDragon->Update();
			enemiesLeft++;
			enemyList.push_back(newDragon);
		}
	}
}

void GameWorld::Update()
{
	for (int i = 0; i < ballList.size(); i++) //Update cannonballs
	{
		ballList.at(i)->Update();
		shotsFired = ballList.size();
	}

	for (int i = 0; i < enemyList.size(); i++) //Update dragon targets
	{
		enemyList.at(i)->Update();
	}

	for (int i = 0; i < ballList.size(); i++) //Update for collisions
	{
		for (int j = 0; j < enemyList.size(); j++)
		{
			if (ballList.at(i)->GetCollider()->GetCollision(enemyList.at(j)->GetCollider()) && enemyList.at(j)->GetAlive())
			{
				// How much momentum do we have?
				double momx = (enemyList.at(j)->GetMass() * enemyList.at(j)->GetVelocity().x()) + (ballList.at(i)->GetMass() * ballList.at(i)->GetVelocity().x());
				double momy = (enemyList.at(j)->GetMass() * enemyList.at(j)->GetVelocity().y()) + (ballList.at(i)->GetMass() * ballList.at(i)->GetVelocity().y());
				double momz = (enemyList.at(j)->GetMass() * enemyList.at(j)->GetVelocity().z()) + (ballList.at(i)->GetMass() * ballList.at(i)->GetVelocity().z());
				// solve equation
				double newvx = momx / (enemyList.at(j)->GetMass() + ballList.at(i)->GetMass());
				double newvy = momy / (enemyList.at(j)->GetMass() + ballList.at(i)->GetMass());
				double newvz = momz / (enemyList.at(j)->GetMass() + ballList.at(i)->GetMass());

				ballList.at(i)->SetVelocity(Vec3(newvx, newvy, newvz));
				enemyList.at(j)->SetVelocity(Vec3(newvx, newvy, newvz));
				enemyList.at(j)->SetAlive(false);
				hits++;
				enemiesLeft--;
				std::cout << "Hits: " << hits << "\n";
			}
		}
	}

	//If all enemies hit
	if (enemiesLeft < 1)
	{
		float accuracy = ((float)hits / (float)shotsFired) * 100;
		std::cout << "Hit Accuracy: " << accuracy << "%\n";
	}
}
