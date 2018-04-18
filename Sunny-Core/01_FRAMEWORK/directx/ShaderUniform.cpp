#include "ShaderUniform.h"

namespace sunny
{
	namespace directx
	{
		/* ShaderUniformDeclaration */

		ShaderUniformDeclaration::ShaderUniformDeclaration(Type type, const std::string& name, unsigned int count) : m_type(type)
		{
			m_size = SizeOfUniformType(type) * count;
			m_name = name;
			m_count = count;
			m_offset = 0;
		};

		ShaderUniformDeclaration::ShaderUniformDeclaration(ShaderStruct* shaderStruct, const std::string& name, unsigned int count)
			: m_type(), m_struct(shaderStruct)
		{
			m_size = m_struct->GetSize();
			m_name = name;
			m_count = count;
			m_offset = 0;
		};

		ShaderUniformDeclaration::Type ShaderUniformDeclaration::StringToType(const std::string& type)
		{
			if (type == "float")    return ShaderUniformDeclaration::Type::FLOAT32;
			if (type == "float2")   return ShaderUniformDeclaration::Type::VEC2;
			if (type == "float3")   return ShaderUniformDeclaration::Type::VEC3;
			if (type == "float4")   return ShaderUniformDeclaration::Type::VEC4;
			if (type == "float3x3") return ShaderUniformDeclaration::Type::MAT3;
			if (type == "float4x4") return ShaderUniformDeclaration::Type::MAT4;
			if (type == "int32")    return ShaderUniformDeclaration::Type::FLOAT32;

			return ShaderUniformDeclaration::Type::NONE;
		}

		std::string ShaderUniformDeclaration::TypeToString(ShaderUniformDeclaration::Type type)
		{
			switch (type)
			{
			case ShaderUniformDeclaration::Type::FLOAT32: return "float";
			case ShaderUniformDeclaration::Type::VEC2:    return "float2";
			case ShaderUniformDeclaration::Type::VEC3:    return "float3";
			case ShaderUniformDeclaration::Type::VEC4:    return "float4";
			case ShaderUniformDeclaration::Type::MAT3:    return "float3x3";
			case ShaderUniformDeclaration::Type::MAT4:    return "float4x4";
			case ShaderUniformDeclaration::Type::INT32:   return "int32";
			default:                                      return "";
			}
		}

		unsigned int ShaderUniformDeclaration::SizeOfUniformType(ShaderUniformDeclaration::Type type)
		{
			switch (type)
			{
			case ShaderUniformDeclaration::Type::FLOAT32: return 4;
			case ShaderUniformDeclaration::Type::VEC2:    return 4 * 2;
			case ShaderUniformDeclaration::Type::VEC3:    return 4 * 3;
			case ShaderUniformDeclaration::Type::VEC4:    return 4 * 4;
			case ShaderUniformDeclaration::Type::MAT3:    return 4 * 3 * 3;
			case ShaderUniformDeclaration::Type::MAT4:    return 4 * 4 * 4;
			case ShaderUniformDeclaration::Type::INT32:   return 4;
			default:                                      return 0;
			}
		}


		/* ShaderUniformBufferDeclaration */

		ShaderUniformBufferDeclaration::ShaderUniformBufferDeclaration(const std::string& name, unsigned int bufferRegister, unsigned int shaderType)
		: m_name(name), m_register(bufferRegister), m_shaderType(shaderType), m_size(0)
		{
		};

		void ShaderUniformBufferDeclaration::PushUniform(ShaderUniformDeclaration* uniform)
		{
			unsigned offset = 0;

			if (m_uniforms.size())
			{
				ShaderUniformDeclaration* previous = static_cast<ShaderUniformDeclaration*>(m_uniforms.back());
				offset = previous->m_offset + previous->m_size;
			}

			uniform->m_offset = offset;
			m_size += uniform->GetSize();
			m_uniforms.push_back(uniform);
		};

		void ShaderUniformBufferDeclaration::Align()
		{
			const int BUFFER_ALIGNMENT = 16;

			m_size = (m_size + (BUFFER_ALIGNMENT - 1)) / BUFFER_ALIGNMENT * BUFFER_ALIGNMENT;
		};

		ShaderUniformDeclaration* ShaderUniformBufferDeclaration::FindUniform(const std::string name)
		{
			for (ShaderUniformDeclaration* uniform : m_uniforms)
			{
				if (uniform->GetName() == name) return uniform;
			}

			return nullptr;
		}

	}
}