#ifndef _DRAGON_
#define _DRAGON_

#include "DynamicObject.h"

using namespace osg;

class GameWorld;
class Collider;

class Enemy : public DynamicObject
{
private:
	Vec3 m_position;
	Vec3 m_acceleration;
	Vec3 m_velocity;
	Vec3 m_center;
	double m_radius;
	double m_angleVelocity;
	double m_dragonTimer;
	double m_size;
	double m_friction, m_bounce;
	double m_verySmall, m_totalTime;
	double m_mass;
	GameWorld *m_pGameWorld;
	Collider *m_pCollider;

	bool m_bAlive, m_bProjectile, m_bCollided;
public:
	Enemy(GameWorld *pGameWorld, double x, double y, double z);

	Vec3 GetCentre() { return m_center; }
	Vec3 GetPosition() { return m_position; }
	Vec3 GetVelocity(){ return m_velocity; };
	double GetSize() { return m_size; }
	Collider* GetCollider() { return m_pCollider; }
	double GetMass() { return m_mass; }

	void SetVelocity(Vec3 newVel);
	void SetPosition(Vec3 newPos);

	void SetAlive(bool alive){ m_bAlive = alive; }
	bool GetAlive() { return m_bAlive; }

	void Update();
	void InitGraphics();

};

#endif