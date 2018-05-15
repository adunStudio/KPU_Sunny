#pragma once

#include <SUNNY.h>

namespace game
{
	class Animation3D : public Entity
	{
	private:
		vector<Mesh*> m_meshs;
		
		int m_preAnimationIndex;

		bool m_roll;

		int m_coolTime;
	
	public:
		Animation3D(vector<Mesh*>& meshs, directx::Texture* texture1, directx::Texture* texture2);

		void SetAnimation(int index);

		inline int GetAnimation() { return m_preAnimationIndex; }

		virtual void PlayAnimation() override;

		inline void SetRoll(bool isRoll) { m_roll = isRoll; }

		inline bool IsRoll() { return m_roll; }

		void Update()
		{
			m_coolTime -= 1;
		}

		inline bool Attack()
		{
			if (m_roll) return false;
			if (m_coolTime <= 0)
			{
				m_coolTime = 20;
				return true;
			}

			return false;
		}
	};
}