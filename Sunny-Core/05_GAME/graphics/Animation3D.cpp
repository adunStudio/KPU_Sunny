#include "Animation3D.h"

namespace game
{
	Animation3D::Animation3D(vector<Mesh*>& meshs, directx::Texture* texture1, directx::Texture* texture2)
		: Entity(meshs[0], texture1, texture2), m_meshs(meshs), m_preAnimationIndex(0), m_roll(false), m_coolTime(0)
	{

	}

	void Animation3D::SetAnimation(int index)
	{
		if (m_preAnimationIndex != index)
			m_frame = 0;
		else
			return;

		m_mesh = m_meshs[index];
		
		m_preAnimationIndex = index;
	}

	void Animation3D::PlayAnimation()
	{
		if (m_mesh->GetType() == MESH_TYPE::STATIC)
			return;

		m_frame++;

		if (m_frame > m_mesh->GetAnimationLength() - 1)
		{
			m_frame = 0;

			if (m_roll) m_roll = false;
			
		}

		m_mesh->SetFrame(m_frame);
	}
}