#include "BulletSystem.h"



BulletSystem::BulletSystem(Model* mesh, Layer3D* layer)
{
	m_mesh = mesh;
	m_layer = layer;
}


BulletSystem::~BulletSystem()
{
	for (auto iter = bullets.begin();iter != bullets.end();)
	{
		delete *iter;
		bullets.erase(iter);
	}
	bullets.clear();
}


void BiDirectionalSystem::Update(float elapsedTime)
{
	m_firerate += elapsedTime;

	if (m_firerate > 0.5)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int i = 0; i < multiShotCount; ++i)
			{
				CBullet* newBullet = new CBullet(m_mesh, m_speed, m_angle[j] + float(i) / multiShotCount);
				m_layer->Add(newBullet->GetEntity());
				bullets.push_back(newBullet);

			}
			m_angle[j] += m_shotAnglerate[j];
			m_angle[j] -= floor(m_angle[j]);
		}
	
		m_firerate = 0;
	}

	for (auto& CBullet : bullets)
	{
		CBullet->Update(elapsedTime);
	}
}

void BentSystem::Update(float elapsedTime)
{
	m_firerate += elapsedTime;

	if (m_firerate > 0.1)
	{
			for (int i = 0; i < multiShotCount; ++i)
			{
				CBullet* newBullet = new CBullet(m_mesh,m_speed, m_angle + (float(i) / multiShotCount), m_speedAcceleration, m_angleAcceleration);
				m_layer->Add(newBullet->GetEntity());
				bullets.push_back(newBullet);
			}

		m_firerate = 0;
	}

	for (auto& CBullet : bullets)
	{
		CBullet->Update(elapsedTime);
	}
}

void BulletGenerator3::Update(float elapsedTime)
{
	m_firerate += elapsedTime;

	if (m_firerate > 0.5)
	{
		for (int i = 0; i < multiShotCount; ++i)
		{
			CBullet* newBullet = new CBullet(m_mesh, m_speed, m_angle + m_angleRange * (float(i) / (multiShotCount - 1) - 0.5f));
			m_layer->Add(newBullet->GetEntity());
			bullets.push_back(newBullet);
		}

		m_firerate = (rand() / (float)RAND_MAX)/3;
	}

	for (auto& CBullet : bullets)
	{
		CBullet->Update(elapsedTime);
	}
}



void AimingNWAYSystem::Update(float elapsedTime)
{
	m_firerate += elapsedTime;
	m_totalTime += elapsedTime;

	if (m_firerate > 0.5&&m_totalTime<2)
	{
		for (int i = 0; i < multiShotCount; ++i)
		{
			CBullet* newBullet = new CBullet(m_mesh, m_speed, m_angle + m_angleRange * (float(i) / (multiShotCount - 1) - 0.5f));
			m_layer->Add(newBullet->GetEntity());
			bullets.push_back(newBullet);
		}

		m_firerate = 0;
	}
	else if(m_totalTime<4)
	{

	}
	else
	{
		m_totalTime = 0;
	}


	for (auto& CBullet : bullets)
	{
		CBullet->Update(elapsedTime);
	}
}

void BulletGenerator9::Update(float elapsedTime)
{
	m_firerate += elapsedTime;

	if (m_firerate > 0.3)
	{
		CBullet* newBullet=nullptr;
		int firecount = rand() % 5+1;
		while (firecount > 0)
		{
			newBullet = new CBullet(m_mesh, m_speed, m_angle + m_angleRange * ((rand() / (float)RAND_MAX) - 0.5f));
			m_layer->Add(newBullet->GetEntity());
			bullets.push_back(newBullet);
			firecount--;
		}



		m_firerate = 0;
	}


	for (auto& CBullet : bullets)
	{
		CBullet->Update(elapsedTime);
	}
}

void BulletGenerator2::Update(float elapsedTime)
{
	m_firerate += elapsedTime;
	m_totalTime += elapsedTime;
	static int index = 0;
	if (m_firerate > 0.25)
	{
		char *p = pattern + index*width;
		for (int i = width - 1; i >= 0; i--, p++)
		{
			if (*p != ' ')
			{
				CBullet* newBullet = new CBullet(m_mesh, m_speed, m_angle+m_angleRange* ((float)i/(width-1)-0.5f) );
				m_layer->Add(newBullet->GetEntity());
				bullets.push_back(newBullet);
			}
		}
		
		m_firerate = 0;
		index += 1;
	}

	for (auto& CBullet : bullets)
	{
		CBullet->Update(elapsedTime);
	}

	index = int(index) % height;
}
