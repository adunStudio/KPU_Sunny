#include "ParticleLayer3D.h"

ParticleLayer3D::ParticleLayer3D()
{

}

ParticleLayer3D::~ParticleLayer3D()
{

}

void ParticleLayer3D::ParticleLayer3D::OnInit(Renderer3D& renderer)
{
	m_particle = new ParticleSystem(new directx::Texture2D("/TEXTURE/star.dds"));
	Add(m_particle);

	Entity* xAxis = new Entity(MeshFactory::CreateXAxis(), RGBA(1, 0, 0, 1), mat4::Identity() * mat4::Scale(vec3(100, 100, 100)));
	Entity* yAxis = new Entity(MeshFactory::CreateYAxis(), RGBA(0, 1, 0, 1), mat4::Identity() * mat4::Scale(vec3(100, 100, 100)));
	Entity* zAxis = new Entity(MeshFactory::CreateZAxis(), RGBA(0, 0, 1, 1), mat4::Identity() * mat4::Scale(vec3(100, 100, 100)));
	AddStatic(xAxis);
	AddStatic(yAxis);
	AddStatic(zAxis);
}

void ParticleLayer3D::ParticleLayer3D::OnTick()
{

}

void ParticleLayer3D::ParticleLayer3D::OnUpdate(const utils::Timestep& ts)
{
	m_particle->Update(ts);
}