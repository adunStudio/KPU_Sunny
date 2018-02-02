#include "ShaderResource.h"

namespace sunny
{
	namespace directx
	{
		ShaderResourceDeclaration::ShaderResourceDeclaration(Type type, const std::string& name, unsigned int count)
		: m_type(type), m_name(name), m_count(count)
		{
		}

		ShaderResourceDeclaration::Type ShaderResourceDeclaration::StringToType(const std::string& type)
		{
			if (type == "Texture2D")    return ShaderResourceDeclaration::Type::TEXTURE2D;
			if (type == "TextureCube")	return ShaderResourceDeclaration::Type::TEXTURECUBE;
			if (type == "SamplerState")	return ShaderResourceDeclaration::Type::SAMPLERSTATE;
			return ShaderResourceDeclaration::Type::NONE;
		}
		
		std::string ShaderResourceDeclaration::TypeToString(Type type)
		{
			switch (type)
			{
			case ShaderResourceDeclaration::Type::TEXTURE2D:    return "Texture2D";
			case ShaderResourceDeclaration::Type::TEXTURECUBE:  return "TextureCube";
			case ShaderResourceDeclaration::Type::SAMPLERSTATE: return "TextureState";
			default: return "";
			}
		}
	}
}