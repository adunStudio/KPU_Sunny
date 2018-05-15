#include "CBullet.h"



CBullet::CBullet()
{
}

CBullet::CBullet(Model * mesh, float speed, float angle, Point startPosition, float speedAccel, float angleAccel)
{
	m_mesh = mesh;
	m_entity = new Entity(m_mesh->GetMesh(), RGBA(1.0, 0, 0, 1.0));
	m_speed = speed;
	m_angle = angle;
	m_speedAccelation = speedAccel;
	m_angleAccelation = angleAccel;
	m_entity->GetTransformComponent()->SetPosition({ startPosition.x, 0, startPosition.z });
}



CBullet::~CBullet()
{
}

void CBullet::Update(float elapsedTime)
{
	if (ready)
	{
		auto currentPosition = m_entity->GetTransformComponent()->GetPosition();
		if ((currentPosition.x > MAX_X || currentPosition.x<-MAX_X || currentPosition.z>MAX_Z || currentPosition.z < -MAX_Z))
		{
			m_entity->SetVisible(false);
		}
		else
		{
			float rad = m_angle * 3.141592 * 2;
			float xMove = m_speed * cosf(rad) * elapsedTime;
			float zMove = m_speed * sinf(rad) * elapsedTime;
			position.x += xMove;
			position.z += zMove;

			m_angle += m_angleAccelation * elapsedTime;
			m_speed += m_speedAccelation * elapsedTime;
			m_entity->GetTransformComponent()->Translate({ xMove,0,zMove });
		}
	}
}

void WaitingBullet::Update(float elapsedTime)
{
	m_waitTime += elapsedTime;
	if (m_waitTime > 2)
	{
		CBullet::Update(elapsedTime);
	}
}
