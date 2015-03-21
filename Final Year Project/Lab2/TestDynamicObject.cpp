#include "stdafx.h"

#include "TestDynamicObject.h"
#include "GameWorld.h"

TestDynamicObject::TestDynamicObject(GameWorld *pGameWorld) 
{
	m_pGameWorld = pGameWorld; 
	// add some graphics to the scene graph node
	InitGraphics();
	// add the scene graph node to the main scenegraph
	m_pGameWorld->GetSceneGraph()->addChild(m_pSceneGraphNode);
}

void TestDynamicObject::Update(osg::Vec3 pos)
{
	//if (rand() < RAND_MAX/64) // once every 64 updates on average
	//{
	//	// pick a random position
	//	osg::Vec3 randPos;
	//	randPos[0] = -100.0f + 200.0f * (float)rand()/(float)RAND_MAX;
	//	randPos[1] = -100.0f + 200.0f * (float)rand()/(float)RAND_MAX;
	//	randPos[2] = 20.0f; // somewhere up in the air
	//	// set the position of the dynamic object
	//	m_pos = randPos;
	//}

	m_pos = pos;
}

void TestDynamicObject::InitGraphics(void)
{
	// a big green sphere
	osg::ref_ptr<osg::Sphere> pSphere = new osg::Sphere( osg::Vec3( 0.0, 0.0, 0.0 ), 0.1);
	osg::ref_ptr<osg::ShapeDrawable> sphereDrawable = new osg::ShapeDrawable( pSphere );
	sphereDrawable->setColor( osg::Vec4( 0.0, 1.0, 0.0, 1.0 ) );
	osg::ref_ptr<osg::Geode> geode = new osg::Geode(); 
	geode->addDrawable( sphereDrawable );
	// add the geode to the scene graph node controlled by the dynamic object
	m_pSceneGraphNode->addChild( geode );
}