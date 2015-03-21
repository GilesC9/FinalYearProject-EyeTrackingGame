#include "Collider.h"
#include "MathHelp.h"
#include "Cannonball.h"
#include "Enemy.h"

//Constructor
Collider::Collider(float radius, osg::Vec3 position)
{
	m_radius = radius;
	m_position = position;
	m_bCollision = false;
}

//Getters
float Collider::GetRadius() { return m_radius; }
osg::Vec3 Collider::GetPosition() { return m_position; }
//Setters
void Collider::SetRadius(float radius){ m_radius = radius; }
void Collider::SetPosition(osg::Vec3 position){ m_position = position; }


bool Collider::GetCollision(Collider* col)
{
	float distance = 0;
	distance = Distance(m_position, col->GetPosition());

	if (distance < (m_radius + col->GetRadius()))
	{
		return true;
	}
	else
		return false;
}



