#pragma once

#include "../include.h"

namespace sunny
{
	namespace graphics
	{
		class Entity;
	}

	namespace component
	{
		struct ComponentType
		{
			std::string name;
		};

		class Component
		{
		protected:
			graphics::Entity* m_entity;

		public:
			virtual graphics::Entity* GetEntity() { return m_entity; }

			virtual ComponentType* GetType() const { return nullptr; }
		};
	}
}