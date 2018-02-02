#pragma once

#include "../sunny.h"
#include "Common.h"

namespace sunny
{
	namespace directx
	{
		class ShaderUniformBufferDeclaration;
		class ShaderStruct;


		/* ShaderUniformDeclaration */
		class ShaderUniformDeclaration
		{
		private:
			friend class Shader;
			friend class ShaderSturct;
			friend class ShaderUniformBufferDeclaration;

		public:
			enum class Type
			{
				NONE, FLOAT32, VEC2, VEC3, VEC4, MAT3, MAT4, INT32
			};

		private:
			std::string  m_name;
			unsigned int m_size;
			unsigned int m_count;
			unsigned int m_offset;
			Type         m_type;
			unsigned int m_register;   // TODO : delete?

			ShaderStruct* m_struct;

		public:
			ShaderUniformDeclaration(Type type, const std::string& name, unsigned int count = 1);
			ShaderUniformDeclaration(ShaderStruct* shaderStruct, const std::string& name, unsigned int count = 1);

			inline const std::string   GetName()     const { return m_name;     }
			inline unsigned int        GetSize()     const { return m_size;     }
			inline unsigned int        GetCount()    const { return m_count;    }
			inline unsigned int        GetOffset()   const { return m_offset;   }
			inline Type                GetType()     const { return m_type;     }
			inline unsigned int        GetRegister() const { return m_register; }
			inline const ShaderStruct* GetStruct()   const { return m_struct;   }

			inline void SetOffset(unsigned int offset) { m_offset = offset; }

		public:
			static        Type  StringToType(const std::string& type);
			static std::string  TypeToString(Type type);
			static unsigned int SizeOfUniformType(Type type);
		};

		typedef std::vector<ShaderUniformDeclaration*> ShaderUniformList;


		/* ShaderUniformBufferDeclaration */
		class ShaderUniformBufferDeclaration
		{
		private:
			friend class Shader;
		
		public:
			std::string m_name;
			ShaderUniformList m_uniforms;
			unsigned int m_register;
			unsigned int m_size;
			unsigned int m_shaderType; // 0 = VS, 1 = PS

		public:
			ShaderUniformBufferDeclaration(const std::string& name, unsigned int bufferRegister, unsigned int shaderType);

			void PushUniform(ShaderUniformDeclaration* uniform);
			
			void Align();

			inline const std::string GetName()       const { return m_name;       }
			inline unsigned int      GetRegister()   const { return m_register;   }
			inline unsigned int      GetShaderType() const { return m_shaderType; }
			inline unsigned int      GetSize()       const { return m_size;       }

			inline const ShaderUniformList& GetUniformDeclarations() const { return m_uniforms; }

			ShaderUniformDeclaration* FindUniform(const std::string name);
		};

		typedef std::vector<ShaderUniformBufferDeclaration*> ShaderUniformBufferList;


		/* ShaderStruct */
		class ShaderStruct
		{
		private:
			friend class Shader;

		private:
			std::string m_name;
			std::vector<ShaderUniformDeclaration*> m_fields;
			unsigned int m_size;
			unsigned int m_offset;

		public:
			ShaderStruct(const std::string& name)
				: m_name(name), m_size(0), m_offset(0)
			{}

			void AddField(ShaderUniformDeclaration* field)
			{
				m_size += field->GetSize();
				unsigned int offset = 0;

				if (m_fields.size())
				{
					ShaderUniformDeclaration* previous = m_fields.back();
					offset = previous->GetOffset() + previous->GetSize();
				}

				field->SetOffset(offset);
				m_fields.push_back(field);
			}

			inline void SetOffset(unsigned int offset) { m_offset = offset; }

			inline const std::string& GetName() const { return m_name; }
			inline unsigned int       GetSize() const { return m_size; }
			inline unsigned int     GetOffset() const { return m_offset; }

			inline const std::vector<ShaderUniformDeclaration*>& GetFields() const { return m_fields; }
		};

		typedef std::vector<ShaderStruct*> ShaderStructList;
		
	}
}