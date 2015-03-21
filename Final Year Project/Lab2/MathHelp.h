#include "stdafx.h"

using namespace osg;

float Distance(Vec3 a, Vec3 b)
{
	return sqrtf(((a.x() - b.x()) * (a.x() - b.x())) + (((a.y() - b.y()) * (a.y() - b.y()))) + (((a.z() - b.z()) * (a.z() - b.z()))));
}

float Distance(Vec2 a, Vec2 b)
{
	return sqrtf(((a.x() - b.x()) * (a.x() - b.x())) + (((a.y() - b.y()) * (a.y() - b.y()))));
}