#include "stdafx.h"
#include "Cannonball.h"
#include "GameWorld.h"

Cannonball::Cannonball(GameWorld *pGameWorld, double x, double y, double z)
{
	DynamicObject();

	m_pGameWorld = pGameWorld;
	m_pCollider = new Collider(0.1, Vec3(x, y, z));
	InitGraphics();

	m_bStationary = true;
	m_bProjectile = false;
	m_size = 0.1;
	m_mass = 1.0f;

	m_position = Vec3(x, y, z);
	m_velocity = Vec3(0, 0, 0);
	m_speed = 0;
	m_angleXZ = 0;
	m_angleYZ = 0;
	m_initialSpeed = 120;

	m_verySmall = 0.001;
	m_totalTime = 0.0;

	m_friction = 0.2;
	m_bounce = 0.4;


	//m_pos dynamic object position
	m_pos[0] = m_position.x();
	m_pos[1] = m_position.y();
	m_pos[2] = m_position.z();

	m_pGameWorld->GetSceneGraph()->addChild(GetSceneGraphNode());
}

void Cannonball::Fire(osg::Vec3 direction)
{
	if (!m_bStationary)
		return;

	m_bStationary = false;
	m_bProjectile = true;

	m_velocity.x() = m_initialSpeed * direction.x();
	m_velocity.y() = m_initialSpeed * direction.y();
	m_velocity.z() = m_initialSpeed * direction.z();

	//m_velocity.x() = m_initialSpeed * cos((2 * PI * m_angleYZ / 360)) * sin((2 * PI * m_angleXZ / 360));
	//m_velocity.y() = m_initialSpeed * sin((2 * PI * m_angleYZ / 360));
	//m_velocity.z() = m_initialSpeed * cos((2 * PI * m_angleYZ / 360)) * cos((2 * PI * m_angleXZ / 360));
}
void Cannonball::AlterPower(double xx)
{
	if (!m_bStationary)
		return;

	m_initialSpeed += xx;

	if (m_initialSpeed < 0)
		m_initialSpeed = 0;
}

void Cannonball::SetVelocity(Vec3 newVel){ m_velocity = newVel; }
void Cannonball::SetPosition(Vec3 newPos){ m_position = newPos; }
void Cannonball::SetCollision(bool status) { m_bCollided = status; }

void Cannonball::RotateInXZPlane(double xx)
{
	if (!m_bStationary)
		return;

	m_angleXZ += xx;
}
void Cannonball::RotateInYZPlane(double xx)
{
	if (!m_bStationary)
		return;

	m_angleYZ += xx;
}
void Cannonball::Reset()
{
	m_bStationary = true;
	m_bProjectile = false;
	m_size = 0.1;

	m_position = Vec3(0, 0, 0);
	m_velocity = Vec3(0, 0, 0);
	//m_acceleration.z() = m_pGameWorld->GetGravity();
	m_speed = 0;
	m_angleXZ = 0;
	m_angleYZ = 0;
	m_initialSpeed = 0;

	m_verySmall = 0.001;
	m_totalTime = 0.0;

	m_friction = 0.7f;
	m_bounce = 0.5;

	m_pos[0] = m_position.x();
	m_pos[1] = m_position.y();
	m_pos[2] = m_position.z();
}

void Cannonball::Info()
{
	// if(stationary==false)return;
	std::cout << "speed=" << m_initialSpeed << " anglexz=" << m_angleXZ << " angleyz=" << m_angleYZ << "\n";
	double vya = m_initialSpeed*sin(m_angleYZ);
	double vxa = m_initialSpeed*cos(m_angleYZ)*cos(m_angleXZ);
	double vza = m_initialSpeed*cos(m_angleYZ)*sin(m_angleXZ);
	std::cout << "corresponds to " << "vx=" << vxa << " vy=" << vya << " vz=" << vza << "\n";
	std::cout << "----------------------------------\n";
	//    cout << "dragon at " << dragon1.getpx() << "," << dragon1.getpy() << "," << dragon1.getpz() << "\n"; 
	//cout << "cannonball at " << px << "," << py << "," << pz << "\n"; 
}

void Cannonball::Update()
{
	if (m_bStationary)
		return;

	m_totalTime += m_pGameWorld->GetDT();

	osg::Vec3 velPost;
	osg::Vec3 normalAtPoint;

	if (m_position.z() < m_pGameWorld->GetTerrain()->HeightAtPoint(m_position.x(), m_position.y()) + m_size)
	{
		normalAtPoint = m_pGameWorld->GetTerrain()->NormalAtPoint(m_position.x(), m_position.y());
		velPost.x() = (normalAtPoint * m_velocity) * normalAtPoint.x();
		velPost.y() = (normalAtPoint * m_velocity) * normalAtPoint.y();
		velPost.z() = (normalAtPoint * m_velocity) * normalAtPoint.z();

		m_velocity.x() = -velPost.x() * m_bounce;
		m_velocity.y() = -velPost.y() * m_bounce;
		m_velocity.z() = -velPost.z() * m_bounce;

		m_position.z() = m_pGameWorld->GetTerrain()->HeightAtPoint(m_position.x(), m_position.y()) + m_size;
		m_bProjectile = false;
	}
	else
		m_bProjectile = true;

	if (m_bProjectile)
	{
		//Under influence of gravity
		m_velocity.z() += m_pGameWorld->GetGravity() * m_pGameWorld->GetDT();
		m_position += m_velocity * m_pGameWorld->GetDT();
	}
	else
	{
		//Get angle of normal against up.
		osg::Vec3 up = osg::Vec3(0.0, 0.0, 1.0);
		normalAtPoint = m_pGameWorld->GetTerrain()->NormalAtPoint(m_position.x(), m_position.y());
		float angleRad = (normalAtPoint * up) / (normalAtPoint.length() * up.length());
		float angleDeg = osg::RadiansToDegrees(angleRad);

		velPost.x() = m_velocity.x() - (normalAtPoint * m_velocity) * normalAtPoint.x();
		velPost.y() = m_velocity.y() - (normalAtPoint * m_velocity) * normalAtPoint.y();
		velPost.z() = m_velocity.z() - (normalAtPoint * m_velocity) * normalAtPoint.z();

		m_velocity.z() = (m_pGameWorld->GetGravity() * sin(angleDeg));

		m_position.x() += m_velocity.x() * m_pGameWorld->GetDT();// * m_friction;
		m_position.y() += m_velocity.y() * m_pGameWorld->GetDT();// * m_friction;
		m_position.z() += m_velocity.z() * m_pGameWorld->GetDT();// * m_friction;

		//m_position.z() = m_pGameWorld->GetTerrain()->HeightAtPoint(m_position.x(), m_position.y()) + m_size;
	}

	m_pos[0] = m_position.x();
	m_pos[1] = m_position.y();
	m_pos[2] = m_position.z();

	m_pCollider->SetPosition(m_pos);
}

void Cannonball::InitGraphics(void) // KY
{
	osg::ref_ptr<osg::Sphere> pSphere = new osg::Sphere(osg::Vec3(0.0, 0.0, 0.0), 0.5);
	osg::ref_ptr<osg::ShapeDrawable> sphereDrawable = new osg::ShapeDrawable(pSphere);
	sphereDrawable->setColor(osg::Vec4(0.2, 0.2, 0.2, 0.0));
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->addDrawable(sphereDrawable);
	GetSceneGraphNode()->addChild(geode);
}
