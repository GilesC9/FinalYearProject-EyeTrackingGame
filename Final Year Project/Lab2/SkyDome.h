#ifndef __SKYDOME_H__
#define __SKYDOME_H__ 1

#include "stdafx.h"

#define PI 3.141592654f

class SkyDome
{
private:
	char* texturePath;
public:
	SkyDome();
	~SkyDome() {}

	void CreateSkyDome(osg::Geode* pGeode, float radius, int rings, int subdivisons);
};

#endif