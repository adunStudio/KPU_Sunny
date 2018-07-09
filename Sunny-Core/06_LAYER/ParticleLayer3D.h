#pragma once

#include <iostream>
#include <SUNNY.h>

using namespace std;
using namespace sunny;
using namespace graphics;
using namespace directx;
using namespace events;
using namespace maths;

class ParticleLayer3D : public Layer3D
{
private:
	ParticleSystem * m_particle;
	MaterialInstance* m_SkyboxMaterial;

public:
	ParticleLayer3D();
	~ParticleLayer3D();

	void OnInit(Renderer3D& renderer) override;
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;
};