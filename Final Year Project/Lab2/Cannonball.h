#ifndef _CANNONBALL_
#define _CANNONBALL_

#include "DynamicObject.h" // KY
#include "Collider.h"

using namespace osg;

#define PI 3.141592654f

class GameWorld;
class Collider;

class Cannonball : public DynamicObject
{
public:
	Vec3 m_position;
	Vec3 m_acceleration;
	Vec3 m_velocity;
	Collider *m_pCollider;

	double m_size, m_speed, m_angleXZ, m_angleYZ, m_initialSpeed;
	double m_verySmall, m_totalTime;
	double m_friction, m_bounce;
	double m_mass;

	GameWorld *m_pGameWorld;

	bool m_bStationary, m_bProjectile, m_bCollided;
public:
	Cannonball(GameWorld *pGameWorld, double x, double y, double z);

	Vec3 GetPosition(){ return m_position; };
	Vec3 GetVelocity(){ return m_velocity; };
	double GetBallSize(){ return m_size; };
	Collider* GetCollider() { return m_pCollider; }
	bool GetFlightStatus(){ return m_bProjectile; }
	double GetMass() { return m_mass; }

	void SetVelocity(Vec3 newVel);
	void SetPosition(Vec3 newPos);
	bool Collision() { return m_bCollided; }
	void SetCollision(bool status);

	void Fire(osg::Vec3 direction);
	void AlterPower(double xx);

	void RotateInXZPlane(double xx);
	void RotateInYZPlane(double xx);
	void Reset();

	void Info();

	void Update();

	void InitGraphics();
};

#endif