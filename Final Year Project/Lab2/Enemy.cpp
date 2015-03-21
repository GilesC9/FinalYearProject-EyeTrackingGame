#include "stdafx.h"

#include "Enemy.h"
#include "GameWorld.h"

Enemy::Enemy(GameWorld *pGameWorld, double x, double y, double z)
{
	DynamicObject();
	m_size = 1.0;
	m_pCollider = new Collider(m_size, Vec3(x, y, z));
	m_pGameWorld = pGameWorld;
	InitGraphics();
	m_center.x() = x;
	m_position.z() = z;
	m_center.y() = y;
	m_radius = 34.7;
	m_angleVelocity = 0.5;
	m_bAlive = true;
	m_mass = 0.5f;

	m_verySmall = 0.001;
	m_totalTime = 0.0;

	m_friction = 0.2;
	m_bounce = 0.4;

	m_pos.x() = m_position.x();
	m_pos.y() = m_position.y();
	m_pos.z() = m_position.z();

	m_pGameWorld->GetSceneGraph()->addChild(GetSceneGraphNode());
}

void Enemy::SetVelocity(Vec3 newVel){ m_velocity = newVel; }
void Enemy::SetPosition(Vec3 newPos){ m_position = newPos; }

void Enemy::Update()
{
	if (m_bAlive) //Holding pattern
	{
		//m_position.x() = m_center.x() + m_radius * cos(m_angleVelocity * m_totalTime);
		//m_position.y() = m_center.y() + m_radius * sin(m_angleVelocity * m_totalTime);
		m_totalTime += m_pGameWorld->GetDT();

		//m_pos dynamic object position
		m_pos.x() = m_position.x();
		m_pos.y() = m_position.y();
		m_pos.z() = m_position.z();

		m_velocity.x() = m_position.x() * 0.05;
		m_velocity.y() = m_position.y() * 0.05;
		m_velocity.z() = m_position.z() * 0.05;
	}
	else if (!m_bAlive) //Apply physics when dead
	{
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
			normalAtPoint = m_pGameWorld->GetTerrain()->NormalAtPoint(m_position.x(), m_position.y());
			osg::Vec3 up = osg::Vec3(0.0, 0.0, 1.0);
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
	}


	m_pCollider->SetPosition(m_pos);
	//cout << "Enemy position is " << m_pos[0] << " " << m_pos[1] << " " << m_pos[2] << "\n";
}

void Enemy::InitGraphics()
{
	osg::ref_ptr<osg::Sphere> pSphere = new osg::Sphere(osg::Vec3(0.0, 0.0, 0.0), m_size);
	osg::ref_ptr<osg::ShapeDrawable> sphereDrawable = new osg::ShapeDrawable(pSphere);

	sphereDrawable->setColor(osg::Vec4(0.0, 1.0, 0.0, 0.0));
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	geode->addDrawable(sphereDrawable);
	GetSceneGraphNode()->addChild(geode);
}
