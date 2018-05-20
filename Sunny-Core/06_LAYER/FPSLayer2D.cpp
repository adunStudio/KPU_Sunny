#include "FPSLayer2D.h"

FPSLayer2D* FPSLayer2D::instance = nullptr;

void FPSLayer2D::SetColor(vec4& color)
{
	if(instance)
		instance->m_fps->SetColor(color);
}

FPSLayer2D::FPSLayer2D()
: Layer2D(maths::mat4::Orthographic(0.0f, Application::GetApplication().GetWindowWidth(), 0.0f, Application::GetApplication().GetWindowHeight(), -1.0f, 1.0f))
{
	m_fps = new Label("fps", m_windowWidth - 5, m_windowHeight - 30, RGBA(1, 1, 1, 0.7), Label::Alignment::RIGHT);

	Add(m_fps);

	instance = this;
}

FPSLayer2D::~FPSLayer2D()
{
	delete m_fps;
}

void FPSLayer2D::OnTick()
{
	m_fps->SetText(std::to_string(Application::GetApplication().GetFPS()) + "fps");
}