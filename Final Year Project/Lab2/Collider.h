#ifndef _COLLIDER_
#define _COLLIDER_

#include "stdafx.h"

#define PI 3.141592654f


class Collider
{
private:
	float m_radius;
	osg::Vec3 m_position;
	bool m_bCollision;
public:
	Collider(){ m_radius = 10; m_position = osg::Vec3(0, 0, 0); }
	Collider(float radius, osg::Vec3 position);

	~Collider(){}

	//Getters
	float GetRadius();
	osg::Vec3 GetPosition();
	//Setters
	void SetRadius(float radius);
	void SetPosition(osg::Vec3 position);

	bool GetCollision(Collider* col);
};

#endif