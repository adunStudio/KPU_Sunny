# Shader

Sunny Engine은 셰이더 프로그램을 컴파일하고, 정점 버퍼, 상수 버퍼를 설정하는 Shader 클래스와 이와 관련된 다양한 클래스가 존재합니다.

여기서는 각 클래스와 메서드가 무슨 역할을 하는지만 개략적으로 설명하므로 자세한 내용은 소스를 읽어보시기 바랍니다.

---

Shader 클래스는 아래와 같이 생성자에서 3단계를 수행합니다.

```cpp
Shader::Shader(const std::string& name, const std::string& source) : m_name(name)
{
	Load(source);       // 셰이더를 생성한다.
	Parse(source);      // 셰이더 프로그램을 파싱한다.
	CreateBuffers();    // 상수 버퍼를 생성한다.
}
```

Shader::Load 메서드는 Shader::Complie 메서드를 호출해서 반환받은 ID3DBlob 인터페이스에 대한 포인터를 이용하여 셰이더를 생성합니다.

```cpp
void Shader::Load(const std::string& source)
{
	ShaderErrorInfo info;

	m_data.vs = Compile(source, "vs_4_0", "VSMain", info);
	m_data.ps = Compile(source, "ps_4_0", "PSMain", info);

	// 정점 셰이더를 생성한다.
	Context::GetDevice()->CreateVertexShader(m_data.vs->GetBufferPointer(), m_data.vs->GetBufferSize(), NULL, &m_data.vertexShader);

	// 픽셀 셰이더를 생성한다.
	Context::GetDevice()->CreatePixelShader(m_data.ps->GetBufferPointer(), m_data.ps->GetBufferSize(), NULL, &m_data.pixelShader);
}
```

그 다음 단계는 Shader::Parse 메서드를 호출해 셰이더 프로그램 소스로부터 아래와 같은 문자열 키워드를 기준으로 파싱합니다.

```cpp
void Shader::Parse(const std::string& source)
{
	const char* token;
	const char* str;

	std::string src = RemoveComments(source);

	str = src.c_str();
	while (token = utils::FindToken(str, "struct"))
		ParseStruct(utils::GetBlock(token, &str));

	str = src.c_str();
	while (token = utils::FindToken(str, "cbuffer"))
		ParseCBuffer(utils::GetBlock(token, &str));

	str = src.c_str();
	while (token = utils::FindToken(str, "Texture2D"))
		ParseTexture(utils::GetStatement(token, &str));

	str = src.c_str();
	while (token = utils::FindToken(str, "TextureCube"))
		ParseTexture(utils::GetStatement(token, &str));

	str = src.c_str();
	while (token = utils::FindToken(str, "SamplerState"))
		ParseSamplerState(utils::GetStatement(token, &str));
}
```

예로 Shader::ParseStruct 메서드는 다음과 같습니다.

```cpp
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

		std::string type = tokens[index++];
		std::string name = tokens[index++];

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
```

마지막으로 셰이더 파싱한 결과를 바탕으로 상수 버퍼를 생성합니다.

```cpp
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

	// 픽셀 셰이더 상수 버퍼 생략...
}
```

---

Shader 클래스는 다음과 같은 멤버를 포함해서 가지고 있습니다.

```cpp
class Shader
{
private:
  ShaderUniformBufferList m_VSUniformBuffers;                // 정점 셰이더 상수 버퍼에 대한 정보
  ShaderUniformBufferList m_PSUniformBuffers;                // 픽셀 셰이더 상수 버퍼에 대한 정보

  ShaderResourceList m_resources;                            // 텍스처등의 자원

  ShaderStructList m_structs;                                // 구조체 정보

  // 생략
};
```

---

```cpp
class ShaderUniformDeclaration { ... };
typedef std::vector<ShaderUniformDeclaration*> ShaderUniformList;
```

ShaderUniformDeclaration 클래스는 셰이더 프로그램의 변수들을 추상화한 클래스다. 멤버변수로 열거형 Type을 가지며, 종류는 다음과 같다.
- FLOAT32 (float)
- VEC2    (float2)
- VEC3    (float3)
- VEC4    (float4)
- MAT3    (float3x3)
- MAT4    (float4x4)
- INT32   (int32)
- NONE

---

```cpp
class ShaderUniformBufferDeclaration
{
public:
  ShaderUniformList m_uniforms;
// 생략..
}
typedef std::vector<ShaderUniformBufferDeclaration*> ShaderUniformBufferList;
```

ShaderUniformBufferDeclaration 클래스는 셰이더 프로그램의 버퍼의 정보를 추상화한 클래스다. 멤버 변수로 vector<ShaderUniformDelcartion*> ShaderUniformList를 가진다. Shader::CreateBuffers() 메서드는 이 클래스의 정보를 바탕으로 상수버퍼를 생성한다.

---

```cpp
class ShaderStruct { ... };
typedef std::vector<ShaderStruct*> ShaderStructList;
```

ShaderStruct 클래스는 셰이더 프로그램의 struct 정보를 추상화한 클래스다. Shader 클래스는 이 정보를 바탕으로 정점버퍼등을 생성한다.

---

```cpp
class ShaderResourceDeclaration { ... }
```

ShaderResourceDeclaration 클래스는 셰이더 프로그램에서 아래와 같은 자원을 추상화한 클래스다.

- TEXTURE2D
- TEXTURECUBE
- SAMPLERSTATE
- NONE

---
### Commit

[Step 07. Shader]()
