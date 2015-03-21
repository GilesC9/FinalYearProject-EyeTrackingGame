#include "stdafx.h"
#ifndef _TESTDYNAMICOBJECT_INC_
#define _TESTDYNAMICOBJECT_INC_

#include "DynamicObject.h"

class GameWorld;

class TestDynamicObject : public DynamicObject
{
	// test dynamic object. The update function changes the position randomly every so often,
	// and the graphics is initialized as a sphere primitive.
public:
	TestDynamicObject( GameWorld *pGameWorld );
	void Update( osg::Vec3 pos );
private:
	GameWorld *m_pGameWorld;
	void InitGraphics( void );
};
#endif