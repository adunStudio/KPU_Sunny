#pragma once

#include "../include.h"
#include "../maths/maths.h"
#include "../system/FileSystem.h"
#include "Common.h"

namespace sunny
{
	namespace directx
	{
		class ShaderResourceDeclaration
		{
		private:
			friend class Shader;

		public:
			enum class Type
			{
				NONE, TEXTURE2D, TEXTURECUBE, SAMPLERSTATE
			};

		private:
			Type m_type;
			std::string m_name;
			unsigned int m_count;
			unsigned int m_register;

		public:
			ShaderResourceDeclaration(Type type, const std::string& name, unsigned int count = 1);

			inline const std::string& GetName()     const { return m_name;};
			inline       Type         GetType()     const { return m_type; }
			inline       unsigned int GetRegister() const { return m_register; }
			inline       unsigned int GetCount()    const {	return m_count; };

		public:
			static Type        StringToType(const std::string& type);
			static std::string TypeToString(Type type);
		};

		typedef std::vector<ShaderResourceDeclaration*> ShaderResourceList;
	}
}