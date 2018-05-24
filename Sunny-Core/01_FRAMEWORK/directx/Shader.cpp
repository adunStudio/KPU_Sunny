#include "Shader.h"

namespace sunny
{
	namespace directx
	{
		/* STATIC */
		const Shader* Shader::s_currentlyBound = nullptr;

		Shader* Shader::CreateFromFile(const std::string& name, const std::string& filepath, void* address)
		{
			std::string source = system::FileSystem::ReadTextFile(filepath);
	
			Shader* result = address ? new(address) Shader(name, source) : new Shader(name, source);
			result->m_filePath = filepath;

			return result;
		}

		Shader* Shader::CreateFromSource(const std::string& name, const std::string& source)
		{
			return new Shader(name, source, DIMENSION::D2);
		}

		bool Shader::TryCompile(const std::string& source, std::string& error)
		{
			ShaderErrorInfo info;

			if (!Compile(source, "vs_4_0", "VSMain", info))
			{
				error += info.message;
				return false;
			}

			if (!Compile(source, "ps_4_0", "PSMain", info))
			{
				error += info.message;
				return false;
			}

			return true;
		}

		bool Shader::TryCompileFromFile(const std::string& filepath, std::string& error)
		{
			std::string source = system::FileSystem::ReadTextFile(filepath);
			return TryCompile(source, error);
		}

		ID3DBlob* Shader::Compile(const std::string& source, const std::string& profile, const std::string& main, ShaderErrorInfo& info)
		{
			ID3DBlob* shaderBlob;
			ID3DBlob* errorBlob;

			HRESULT status = D3DCompile(source.c_str(), source.size(), NULL, NULL, NULL, main.c_str(), profile.c_str(), D3DCOMPILE_DEBUG, 0, &shaderBlob, &errorBlob);
			// pSrcData    : 컴파일되지 않은 셰이더 데이터에 대한 포인터. ASCII HLSL 코드 또는 컴파일 된 효과.
			// SrcDataSize : pSrcData 길이
			// pSourceName : 사용하지 않을 고급 옵션
			// pDefines    : 사용하지 않을 고급 옵션
			// pinclude    : 사용하지 않을 고급 옵션
			// pEntrypoint : 셰이더 실행이 시작되는 셰이더 진입 점 함수의 이름
			// pTarget     : 사용할 셰이더 버전을 뜻하는 문자열
			// Flags1      : 셰이더 코드의 컴파일 방식에 영향을 미치는 플래그
			// Flags2      : 사용하지 않을 고급 옵션
			// ppCode      : 컴파일 된 코드에 액세스하는 데 사용할 수있는 ID3DBlob 인터페이스에 대한 포인터를받는 변수에 대한 포인터
			// ppErrorMsgs : 컴파일러 오류 메시지에 액세스하는 데 사용할 수있는 ID3DBlob 인터페이스에 대한 포인터를받는 변수에 대한 포인터,  오류가없는 경우 NULL입


			if (status != S_OK)
				info.message = "Unable to compile shader form souce\n";

			if (errorBlob)
			{
				info.profile += profile + "\n";
				if(errorBlob->GetBufferSize())
					std::cout << "Shader Compile Errors" << std::endl << (const char*)errorBlob->GetBufferPointer() << std::endl;
				errorBlob->Release();
			}
			
			if (status == S_OK)
				return shaderBlob;

			return nullptr;
		}

		/* PUBLIC */
		Shader::Shader(const std::string& name, const std::string& source, DIMENSION dimension) : m_name(name), m_dimension(dimension)
		{
			m_VSUserUniformBuffer = nullptr;
			m_PSUserUniformBuffer = nullptr;

			Load(source);       // 셰이더를 생성한다.
			Parse(source);      // 셰이더 프로그램을 파싱한다.
			CreateBuffers();    // 상수 버퍼를 생성한다.
		}

		Shader::~Shader()
		{
			m_data.vertexShader->Release();
			m_data.pixelShader->Release();
		}

		void Shader::Bind() const
		{
			s_currentlyBound = this;

			if (m_dimension == DIMENSION::D3)
			{
				Context::GetDeviceContext()->VSSetShader(m_data.vertexShader, NULL, 0);
				Context::GetDeviceContext()->PSSetShader(m_data.pixelShader, NULL, 0);

				Context::GetDeviceContext()->VSSetConstantBuffers(0, m_VSConstantBuffersCount, m_VSConstantBuffers);
				Context::GetDeviceContext()->PSSetConstantBuffers(0, m_PSConstantBuffersCount, m_PSConstantBuffers);
			}
			else
			{
				Context::GetDeferredDeviceContext()->VSSetShader(m_data.vertexShader, NULL, 0);
				Context::GetDeferredDeviceContext()->PSSetShader(m_data.pixelShader, NULL, 0);

				Context::GetDeferredDeviceContext()->VSSetConstantBuffers(0, m_VSConstantBuffersCount, m_VSConstantBuffers);
				Context::GetDeferredDeviceContext()->PSSetConstantBuffers(0, m_PSConstantBuffersCount, m_PSConstantBuffers);
			}
			
		}

		void Shader::SetVSSystemUniformBuffer(unsigned char* data, unsigned int size, unsigned int slot)
		{
			if (m_VSUserUniformBuffer)
			{
				if (slot == m_VSUserUniformBuffer->GetRegister())
				{
					// Debug System
					std::cout << "DEBUG : Shader::SetVSSystemUniformBuffer-1" << std::endl;
				}
			}

			ID3D11Buffer* cbuffer = m_VSConstantBuffers[slot];
			
			D3D11_MAPPED_SUBRESOURCE msr;
			ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));
			if (m_dimension == DIMENSION::D3)
				Context::GetDeviceContext()->Map(cbuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msr);
			else
				Context::GetDeferredDeviceContext()->Map(cbuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msr);

			// pResource       : ID3D11Resource 인터페이스에 대한 포인터다.
			// SubResource     : 서브 리소스의 인덱스
			// MapType         : 리소스에 대한 CPU의 읽기/쓰기 권한을 나타내는 D3D11_MAP 열겨형 상수다.        
			//    ㄴ             D3D11_MAP_WRITE_DISCARD → 리소스에 쓰려고 맵핑한다. 리소스의 이전 내용은 보존되지 않는다.
			// MapFlags        : GPU가 리로스에 접근하고 있을 때 CPU가 어떻게 할 것인가를 나타내는 상수다.
			// pMappedResource : 맵핑된 서브 리소스에 대한 포인터다.
			memcpy(msr.pData, data, size);
			
			if (m_dimension == DIMENSION::D3)
				Context::GetDeviceContext()->Unmap(cbuffer, NULL);
			else
				Context::GetDeferredDeviceContext()->Unmap(cbuffer, NULL);
		}

		void Shader::SetPSSystemUniformBuffer(unsigned char* data, unsigned int size, unsigned int slot)
		{

			if (m_PSUserUniformBuffer)
			{
				if (slot == m_PSUserUniformBuffer->GetRegister())
				{
					// Debug System
					std::cout << "DEBUG : Shader::SetPSSystemUniformBuffer-1" << std::endl;
				}
			}

			ID3D11Buffer* cbuffer = m_PSConstantBuffers[slot];

			D3D11_MAPPED_SUBRESOURCE msr;
			ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));

			if (m_dimension == DIMENSION::D3)
				Context::GetDeviceContext()->Map(cbuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msr);
			else
				Context::GetDeferredDeviceContext()->Map(cbuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msr);

			// pResource       : ID3D11Resource 인터페이스에 대한 포인터다.
			// SubResource     : 서브 리소스의 인덱스
			// MapType         : 리소스에 대한 CPU의 읽기/쓰기 권한을 나타내는 D3D11_MAP 열겨형 상수다.        
			//    ㄴ             D3D11_MAP_WRITE_DISCARD → 리소스에 쓰려고 맵핑한다. 리소스의 이전 내용은 보존되지 않는다.
			// MapFlags        : GPU가 리로스에 접근하고 있을 때 CPU가 어떻게 할 것인가를 나타내는 상수다.
			// pMappedResource : 맵핑된 서브 리소스에 대한 포인터다.

			memcpy(msr.pData, data, size);
			
			if (m_dimension == DIMENSION::D3)
				Context::GetDeviceContext()->Unmap(cbuffer, NULL);
			else
				Context::GetDeferredDeviceContext()->Unmap(cbuffer, NULL);
		}

		void Shader::SetVSUserUniformBuffer(unsigned char* data, unsigned int size)
		{
			ID3D11Buffer* cbuffer = m_VSConstantBuffers[m_VSUserUniformBuffer->GetRegister()];

			D3D11_MAPPED_SUBRESOURCE msr;
			memset(&msr, 0, sizeof(D3D11_MAPPED_SUBRESOURCE));
			if (m_dimension == DIMENSION::D3)
				Context::GetDeviceContext()->Map(cbuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msr);
			else
				Context::GetDeferredDeviceContext()->Map(cbuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msr);

			memcpy(msr.pData, data, size);

			if (m_dimension == DIMENSION::D3)
				Context::GetDeviceContext()->Unmap(cbuffer, NULL);
			else
				Context::GetDeferredDeviceContext()->Unmap(cbuffer, NULL);
		}

		void Shader::SetPSUserUniformBuffer(unsigned char* data, unsigned int size)
		{
			ID3D11Buffer* cbuffer = m_PSConstantBuffers[m_VSUserUniformBuffer->GetRegister()];

			D3D11_MAPPED_SUBRESOURCE msr;
			memset(&msr, 0, sizeof(D3D11_MAPPED_SUBRESOURCE));

			if (m_dimension == DIMENSION::D3)
				Context::GetDeviceContext()->Map(cbuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msr);
			else
				Context::GetDeferredDeviceContext()->Map(cbuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msr);

			memcpy(msr.pData, data, size);

			if (m_dimension == DIMENSION::D3)
				Context::GetDeviceContext()->Unmap(cbuffer, NULL);
			else
			{
				Context::GetDeferredDeviceContext()->Unmap(cbuffer, NULL);

			}
		}

		/* PRIVATE */
		void Shader::Load(const std::string& source)
		{
			ShaderErrorInfo info;

			m_data.vs = Compile(source, "vs_4_0", "VSMain", info);
			if (!m_data.vs)
			{
				// TODO: Log System
				std::cout << "DEBUG : Shader::Load-1" << std::endl;
				std::cout << info.profile << std::endl;
				std::cout << info.message << std::endl;
			}

			m_data.ps = Compile(source, "ps_4_0", "PSMain", info);
			if (!m_data.ps)
			{
				// TODO: Log System
				std::cout << "DEBUG : Shader::Load-2" << std::endl;
				std::cout << info.profile << std::endl;
				std::cout << info.message << std::endl;
			}

			// 정점 셰이더를 생성한다.
			Context::GetDevice()->CreateVertexShader(m_data.vs->GetBufferPointer(), m_data.vs->GetBufferSize(), NULL, &m_data.vertexShader);
			
			// 픽셀 셰이더를 생성한다.
			Context::GetDevice()->CreatePixelShader(m_data.ps->GetBufferPointer(), m_data.ps->GetBufferSize(), NULL, &m_data.pixelShader);
		}

		// 셰이더 코드에 있는 주석을 제거한다.
		std::string Shader::RemoveComments(const std::string& source)
		{
			const char* str = source.c_str();
			
			std::string result = source;

			int startPos;

			while ((startPos = utils::FindStringPosition(result, "/*")) != -1)
			{
				int endPos = utils::FindStringPosition(result, "*/");

				if (endPos == -1)
				{
					std::cout << "DEBUG : Shader::RemoveComments-3" << std::endl;
					// TODO: Debug System
				}

				result = utils::RemoveStringRange(result, startPos, endPos - startPos + 2);
			}

			while ((startPos = utils::FindStringPosition(result, "//")) != -1)
			{
				int endPos = utils::FindStringPosition(result, "\n", startPos);
				if (endPos == -1)
				{
					std::cout << "DEBUG : Shader::RemoveComments-4" << std::endl;
					// TODO: Debug System
				}

				result = utils::RemoveStringRange(result, startPos, endPos - startPos + 1);
			}
			
			return result;
		}

		// 셰이더 코드를 파싱한다.
		void Shader::Parse(const std::string& source)
		{
			const char* token;
			const char* str;

			std::string src = RemoveComments(source);

			str = src.c_str();

			while (token = utils::FindToken(str, "struct"))
				ParseStruct(utils::GetBlock(token, &str));         // m_structs 생성

			str = src.c_str();
			while (token = utils::FindToken(str, "cbuffer"))
				ParseCBuffer(utils::GetBlock(token, &str));        // m_VSUserUniformBuffer, m_PSUserUniformBuffer, m_VSUserUniformBuffer, m_PSUserUniformBuffer

			str = src.c_str();
			while (token = utils::FindToken(str, "Texture2D"))
				ParseTexture(utils::GetStatement(token, &str));    // m_resources

			str = src.c_str();
			while (token = utils::FindToken(str, "TextureCube"))
				ParseTexture(utils::GetStatement(token, &str));    // m_resources

			str = src.c_str();
			while (token = utils::FindToken(str, "SamplerState"))
				ParseSamplerState(utils::GetStatement(token, &str)); // TODO : 샘플러 처리
		}

		// 구조체 파싱
		void Shader::ParseStruct(const std::string& block)
		{
			std::vector<std::string> tokens = utils::Tokenize(block);
	
			unsigned int index = 0;
			index++; // "struct"

			std::string structName = tokens[index++];
			
			ShaderStruct*  shaderStruct = new ShaderStruct(structName);

			index++; // "{"

			while (index < tokens.size())
			{
				if (tokens[index] == "}")
					break;

				std::string type = tokens[index++];  //ex) float4
				std::string name = tokens[index++];  //ex) position

				if (type == ":")
					continue;

				// ";"
				if (const char* s = strstr(name.c_str(), ";"))
					name = std::string(name.c_str(), s - name.c_str());

				// 정점 버퍼에 대한 정보를 생성한다.
				ShaderUniformDeclaration* field = new ShaderUniformDeclaration(ShaderUniformDeclaration::StringToType(type), name);
				shaderStruct->AddField(field);
			}

			m_structs.push_back(shaderStruct);
		}

		// 상수 버퍼 파싱
		void Shader::ParseCBuffer(const std::string& block)
		{
			std::vector<std::string> tokens = utils::Tokenize(block);

			unsigned int index = 1;

			std::string bufferName = tokens[index++];
			unsigned int reg = 0;

			if (tokens[index++] == ":") // 레지스터
			{
				std::string cbRegister = tokens[index++];
				reg = utils::NextInt(cbRegister);
			}

			ShaderUniformBufferDeclaration* buffer = nullptr;

			unsigned int shaderType;

			if (utils::StartsWith(bufferName, "VS"))
				shaderType = 0;
			else  if (utils::StartsWith(bufferName, "PS"))
				shaderType = 1;
			else
			{
				std::cout << "Exit: Shader.cpp[348]" << std::endl;
				exit(0);// Debug System
			}

			index++;

			while (index < tokens.size())
			{
				if (tokens[index] == "}")
					break;

				std::string type = tokens[index++];
				std::string name = tokens[index++];

				// ; 까지 파싱
				if (const char* s = strstr(name.c_str(), ";"))
					name = std::string(name.c_str(), s - name.c_str());

				if (buffer == nullptr)
				{
					buffer = new  ShaderUniformBufferDeclaration(bufferName, reg, shaderType);

					if (utils::StartsWith(name, "SUNNY_"))
					{
						switch (shaderType)
						{
						case 0:
							m_VSUniformBuffers.push_back(buffer);
							break;
						case 1:
							m_PSUniformBuffers.push_back(buffer);
							break;
						}
					}
					else
					{
						switch (shaderType)
						{
						case 0:
							if (m_VSUserUniformBuffer != nullptr)
							{
								// Debug System
								std::cout << "DEBUG : Shader::ParseCBuffer-1" << std::endl;
							}
							m_VSUserUniformBuffer = buffer;
							break;
						case 1:
							if (m_PSUserUniformBuffer != nullptr)
							{
								// Debug System
								std::cout << "DEBUG : Shader::ParseCBuffer-2" << std::endl;
							}
							m_PSUserUniformBuffer = buffer;
							break;
						}
					}
				}

				ShaderUniformDeclaration::Type t = ShaderUniformDeclaration::StringToType(type);
				ShaderUniformDeclaration* declaration = nullptr;

				if (t == ShaderUniformDeclaration::Type::NONE)
				{
					// ex) 타입 : VSInput
					ShaderStruct* s = FindStruct(type);
					if (!s)
					{
						// Debug System
						std::cout << "DEBUG : Shader::ParseCBuffer-3" << std::endl;
					}

					// s(구조체) 의 사이즈만큼 늘린다.
					declaration = new ShaderUniformDeclaration(s, name);
				}
				else
				{
					declaration = new ShaderUniformDeclaration(t, name);
				}

				buffer->PushUniform(declaration);
			}

			buffer->Align();
		}

		// 텍스처 파싱
		void Shader::ParseTexture(const std::string& statement)
		{
			std::vector<std::string> tokens = utils::Tokenize(statement);

			unsigned int index = 0;
			unsigned int reg = 0;

			std::string type = tokens[index++];
			std::string name = tokens[index++];

			if (tokens[index++] == ":")
			{
				std::string texRegister = tokens[index++];
				reg = utils::NextInt(texRegister);
			}

			ShaderResourceDeclaration* declaration = new ShaderResourceDeclaration(ShaderResourceDeclaration::StringToType(type), name);
			declaration->m_register = reg;

			m_resources.push_back(declaration);
		}

		void Shader::ParseSamplerState(const std::string& statement)
		{
			std::vector<std::string> tokens = utils::Tokenize(statement);
			if (tokens.front() != "SamplerState")
			{
				std::cout << "DEBUG : Shader::ParseSamplerState-1" << std::endl;
				// Debug System
			}

			unsigned int reg = 0;
			unsigned int index = 1;

			std::string name = tokens[index++];

			if (tokens[index++] == ":")
			{
				std::string sampleRegister = tokens[index++];
				reg = utils::NextInt(sampleRegister);
			}
		}

		
		// 파싱한 결과를 근거로 상수 버퍼를 만든다.
		// 상수 버퍼(constant buffer)는 셰이더가 접근할 수 잇는 다양한 자료를 저장하는 유연한 자료 블록이다.
		// 상수 버퍼의 내용을 실행 시점에서 바꿀 수 있다.
		void Shader::CreateBuffers()
		{
			/* 정점 셰이더 상수 버퍼 */
			m_VSConstantBuffersCount = m_VSUniformBuffers.size() + (m_VSUserUniformBuffer ? 1 : 0);
			m_VSConstantBuffers = new ID3D11Buffer*[m_VSConstantBuffersCount];

			for (unsigned int i = 0; i < m_VSUniformBuffers.size(); ++i)
			{
				ShaderUniformBufferDeclaration* decl = static_cast<ShaderUniformBufferDeclaration*>(m_VSUniformBuffers[i]);

				D3D11_BUFFER_DESC desc;
				ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
				desc.ByteWidth = decl->GetSize();
				desc.Usage = D3D11_USAGE_DYNAMIC;
				desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;        // 상수 버퍼
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

				D3D11_SUBRESOURCE_DATA data;
				data.pSysMem = new unsigned char[desc.ByteWidth];
				data.SysMemPitch = 0;
				data.SysMemSlicePitch = 0;

				Context::GetDevice()->CreateBuffer(&desc, &data, &m_VSConstantBuffers[decl->GetRegister()]);
				// pDesc        : 생성할 버퍼를 설정하는 D3D11_BUFFER_DESC 구조체의 포인터
				// pInitialData : 생성할 버퍼를 초기화 하기 위한 데이터(D3D11_SUBRESOURCE_DATA)의 포인터, 이 값이 NULL이면 메모리만 할당하고 초기화하지 않는다. 
				// ppBuffer     : 생성된 버퍼 인터페이스를 반환할 포인터의 주소
			}

			if (m_VSUserUniformBuffer)
			{
				ShaderUniformBufferDeclaration* decl = m_VSUserUniformBuffer;

				D3D11_BUFFER_DESC desc;
				ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
				desc.ByteWidth = decl->GetSize();
				desc.Usage = D3D11_USAGE_DYNAMIC;
				desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

				D3D11_SUBRESOURCE_DATA data;
				data.pSysMem = new byte[desc.ByteWidth];
				data.SysMemPitch = 0;
				data.SysMemSlicePitch = 0;
				Context::GetDevice()->CreateBuffer(&desc, &data, &m_VSConstantBuffers[decl->GetRegister()]);
			}

			/* 픽셀 셰이더 상수 버퍼 */
			m_PSConstantBuffersCount = m_PSUniformBuffers.size() + (m_PSUserUniformBuffer ? 1 : 0);
			m_PSConstantBuffers = new ID3D11Buffer*[m_PSConstantBuffersCount];

			for (unsigned int i = 0; i < m_PSUniformBuffers.size(); ++i)
			{
				ShaderUniformBufferDeclaration* decl = static_cast<ShaderUniformBufferDeclaration*>(m_PSUniformBuffers[i]);

				D3D11_BUFFER_DESC desc;
				ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
				desc.ByteWidth = decl->GetSize();
				desc.Usage = D3D11_USAGE_DYNAMIC;
				desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

				D3D11_SUBRESOURCE_DATA data;
				data.pSysMem = new byte[desc.ByteWidth];
				data.SysMemPitch = 0;
				data.SysMemSlicePitch = 0;
				Context::GetDevice()->CreateBuffer(&desc, &data, &m_PSConstantBuffers[decl->GetRegister()]);
			}

			if (m_PSUserUniformBuffer)
			{
				ShaderUniformBufferDeclaration* decl = m_PSUserUniformBuffer;

				D3D11_BUFFER_DESC desc;
				ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
				desc.ByteWidth = decl->GetSize();
				desc.Usage = D3D11_USAGE_DYNAMIC;
				desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

				D3D11_SUBRESOURCE_DATA data;
				data.pSysMem = new byte[desc.ByteWidth];
				data.SysMemPitch = 0;
				data.SysMemSlicePitch = 0;
				Context::GetDevice()->CreateBuffer(&desc, &data, &m_PSConstantBuffers[decl->GetRegister()]);
			}
		}

		ShaderStruct* Shader::FindStruct(const std::string& name)
		{
			for (ShaderStruct* s : m_structs)
			{
				if (s->GetName() == name)
					return s;
			}

			return nullptr;
		}

	}
}