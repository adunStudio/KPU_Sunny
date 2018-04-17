struct VSInput
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
};

struct VSOutput
{
	float4 positionCS : SV_POSITION;
	float3 cameraPosition : CAMERA_POSITION;
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
	float3 color : COLOR;
	float4 shadowCoord : SHADOW_POSITION;
	float4 lightPosition : LIGHT_POSITION;
};

cbuffer VSSystemUniforms : register(b0)
{
	float4x4 SUNNY_ProjectionMatrix;
	float4x4 SUNNY_ViewMatrix;
	float4x4 SUNNY_ModelMatrix;
	float4x4 SUNNY_LightProjectionMatrix;
	float4x4 SUNNY_LightViewMatrix;
	float3	 SUNNY_CameraPosition;
};

VSOutput VSMain(in VSInput input)
{
	float3x3 wsTransform = (float3x3)SUNNY_ModelMatrix;
	VSOutput output;
	output.position = mul(input.position, SUNNY_ModelMatrix);
	output.positionCS = mul(output.position, mul(SUNNY_ViewMatrix, SUNNY_ProjectionMatrix));
	output.normal = mul(input.normal, wsTransform);
	output.binormal = mul(input.binormal, wsTransform);
	output.tangent = mul(input.tangent, wsTransform);
	output.uv = input.uv;
	output.color = float3(1.0f, 1.0f, 1.0f);
	output.shadowCoord = float4(0.0f, 0.0f, 0.0f, 0.0f); // output.shadowCoord = mul(output.position, depthBias);
	output.lightPosition = mul(output.position, mul(SUNNY_LightViewMatrix, SUNNY_LightProjectionMatrix));
	output.cameraPosition = SUNNY_CameraPosition;

	return output;
}

struct Light
{
	float4 color;
	float3 position;
	float p0;
	float3 ambientDown;
	float p1;
	float3 ambientRange;
	float intensity;
};

struct Attributes
{
	float3 position;
	float2 uv;
	float3 normal;
	float3 binormal;
	float3 tangent;
};


cbuffer PSSystemUniforms : register(b0)
{
	Light SUNNY_Light;
	float4 SUNNY_Color;
	float  SUNNY_HasTexture;
};


float3 CalcAmbient(float3 normal, float3 diffuseColor)
{
	float up = normal.y * 0.5 + 0.5;//노말을 0~1범위로 변환 y축이 상하축과 일치한다고 가정

	float3 ambient = SUNNY_Light.ambientDown + up * SUNNY_Light.ambientRange;//엠비언트 값을 계산

	return ambient * diffuseColor;//엠비언트 값에 색을 곱한다
}

float3 CalcDirectional(float3 worldPosition, float3 worldNormal, float4 diffuseColor, float3 EyePosition)
{
	// Phong diffuse
	float NDotL = dot(SUNNY_Light.position, worldNormal);//노멀과 빛의 방향 벡터를 내적해 난반사광의 양을 구한다.
	float3 finalColor = SUNNY_Light.color.rgb * saturate(NDotL);//내적한 값의 0보다 작거나 1보다 큰 값은 필요없으므로 버리고

																// Blinn specular
	float3 ToEye = EyePosition.xyz - worldPosition;//카메라 위치와 월드 공간에서의 정점을 빼서 카메라 벡터를 구한다.
	ToEye = normalize(ToEye);//정규화
	float3 HalfWay = normalize(ToEye + SUNNY_Light.position);//카메라벡터와 빛의 방향의 중간 벡터를 구하고 정규화
	float NDotH = saturate(dot(HalfWay, worldNormal));//중간벡터와 법선백터를 내적해서 코사인 값을 구하고 0~1범위로 만듬
	finalColor += SUNNY_Light.color.rgb * pow(NDotH, 250.0f) * SUNNY_Light.intensity;//정반사광 계수로 퍼지는 정도를 정하고 세기를 곱해서 최종 정반사광의 양을 구함

	return finalColor * diffuseColor.rgb;//난반사광+정반사과 * 텍셀
}

Texture2D textures : register(t0);
Texture2D shadowMap: register(t7);
SamplerState samplers : register(s0);
SamplerState shadowSampler : register(s7);


float4 PSMain(in VSOutput input) : SV_TARGET
{

	float4 texColor = (float4)SUNNY_Color;

	if (SUNNY_HasTexture >= 1)
	{
		texColor = textures.Sample(samplers, input.uv);
	}

	texColor.rgb *= texColor.rgb; // 더 진해진다.

	float3 world_pos = input.position;
	float3 world_normal = normalize(input.normal);


	float3 color = float3(texColor.x, texColor.y, texColor.z);

	int material_shininess = 100;
	float material_kd = 0.5;
	float material_ks = 0.3;

	int levels = 9;
	float scaleFactor = 1.0f / levels;

	float3 eye_position = input.cameraPosition;
	//cameraPosition
	float3 light_position = float3(input.lightPosition.x, input.lightPosition.y, input.lightPosition.z);

	float3 Kd = float3(0.3f, 0.8f, 0.1f);

	float3 L = normalize(light_position - world_pos);
	float3 V = normalize(eye_position - world_pos);

	float difuza = max(0, dot(L, world_normal));

	Kd = Kd * material_kd * floor(difuza * levels) * scaleFactor;

	float3 H = normalize(L + V);

	float speculara = 0;

	if (dot(L, world_normal) > 0.0)
	{
		speculara = material_ks * pow(max(0, dot(H, world_normal)), material_shininess);
	}

	float specMask = (pow(dot(H, world_normal), material_shininess) > 0.4) ? 1 : 0;
	float edgeMask = (dot(V, world_normal) >  0.2) ? 1 : 1;

	if (SUNNY_HasTexture == 0)
		color = edgeMask * (color + Kd + speculara * specMask);

	
	float3 finalColor =  CalcAmbient(world_normal, color);
	finalColor += CalcDirectional(input.positionCS, world_normal, texColor, input.cameraPosition);

	float bias = 0.000005;

	input.lightPosition.xyz /= input.lightPosition.w;

	if (input.lightPosition.x < -1.0f || input.lightPosition.x > 1.0f ||
		input.lightPosition.y < -1.0f || input.lightPosition.y > 1.0f ||
		input.lightPosition.z <  0.0f || input.lightPosition.z > 1.0f) return float4(finalColor * 0.5, texColor.a);

	input.lightPosition.x = input.lightPosition.x / 2 + 0.5;
	input.lightPosition.y = input.lightPosition.y / -2 + 0.5;

	input.lightPosition.z -= bias;

	float shadowMapDepth = shadowMap.Sample(shadowSampler, input.lightPosition.xy).r;

	if (shadowMapDepth < input.lightPosition.z) return float4(finalColor * 0.5, texColor.a);

	return float4(finalColor, texColor.a);

}

float LineThickness = 0.03;

VSOutput OutlineVertexShader(in VSInput input)
{
	float3x3 wsTransform = (float3x3)SUNNY_ModelMatrix;
	VSOutput output = (VSOutput)0;
	output.position = mul(input.position , SUNNY_ModelMatrix);
	output.positionCS = mul(output.position, mul(SUNNY_ViewMatrix, SUNNY_ProjectionMatrix));
	float4 normal = mul(mul(mul(input.normal, SUNNY_ModelMatrix), SUNNY_ViewMatrix), SUNNY_ProjectionMatrix);
	normal = mul(output.normal, mul(SUNNY_ViewMatrix, SUNNY_ProjectionMatrix));
	
	output.positionCS = output.positionCS + (mul(LineThickness, normal));



	return output;
}


float4 OutlinePixelShader(VSOutput input) : SV_TARGET
{
	return float4(0, 0, 0, 1);
}

technique Toon
{
	// The first pass will go through and draw the back-facing triangles with the outline shader,
	// which will draw a slightly larger version of the model with the outline color.  Later, the
	// model will get drawn normally, and draw over the top most of this, leaving only an outline.
	pass Pass1
	{
		VertexShader = compile vs_2_0 OutlineVertexShader();
		PixelShader = compile ps_2_0 OutlinePixelShader();
		CullMode = CW;
	}

	// The second pass will draw the model like normal, but with the cel pixel shader, which will
	// color the model with certain colors, giving us the cel/toon effect that we are looking for.
	pass Pass2
	{
		VertexShader = compile vs_2_0 VSMain();
		PixelShader = compile ps_2_0 PSMain();
		CullMode = CCW;
	}
}