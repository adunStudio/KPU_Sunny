#include "ParticleLayer3D.h"

ParticleLayer3D::ParticleLayer3D()
{

}

ParticleLayer3D::~ParticleLayer3D()
{

}

void ParticleLayer3D::ParticleLayer3D::OnInit(Renderer3D& renderer)
{
	m_particle = new ParticleSystem(new directx::Texture2D("/TEXTURE/particle.png"));
	Add(m_particle);

	Entity* xAxis = new Entity(MeshFactory::CreateXAxis(), RGBA(1, 0, 0, 1), mat4::Identity() * mat4::Scale(vec3(100, 100, 100)));
	Entity* yAxis = new Entity(MeshFactory::CreateYAxis(), RGBA(0, 1, 0, 1), mat4::Identity() * mat4::Scale(vec3(100, 100, 100)));
	Entity* zAxis = new Entity(MeshFactory::CreateZAxis(), RGBA(0, 0, 1, 1), mat4::Identity() * mat4::Scale(vec3(100, 100, 100)));
	AddStatic(xAxis);
	AddStatic(yAxis);
	AddStatic(zAxis);

	std::string skyBoxFiles[1] =
	{
		"/CUBE/skybox.png",
	};

	TextureCube* environment = new TextureCube("skybox", skyBoxFiles, 1);
	Shader* skybox = Shader::CreateFromFile("skybox", "/HLSL/03_skybox.hlsl");
	Material* skyboxMaterial = new Material(skybox);
	skybox->Bind();
	m_SkyboxMaterial = new MaterialInstance(skyboxMaterial);
	m_SkyboxMaterial->SetTexture("u_EnvironmentMap", environment);
	Entity* skyboxEntity = new Entity(MeshFactory::CreateQuad(-1, -1, 2, 2));
	skyboxEntity->SetMaterial(m_SkyboxMaterial);
	SetSkybox(skyboxEntity);
}

void ParticleLayer3D::ParticleLayer3D::OnTick()
{

}

void ParticleLayer3D::ParticleLayer3D::OnUpdate(const utils::Timestep& ts)
{
	m_particle->Update(ts);

	mat4 vp = GetCamera()->GetProjectionMatrix() * GetCamera()->GetViewMatrix();
	m_SkyboxMaterial->SetUniform("invViewProjMatrix", mat4::Invert(vp));
}