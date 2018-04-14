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
	float3	 SUNNY_CameraPosition;
	float4x4 SUNNY_LightProjectionMatrix;
	float4x4 SUNNY_LightViewMatrix;
};

VSOutput VSMain(in VSInput input)
{
	float3x3 wsTransform = (float3x3)SUNNY_ModelMatrix;

	VSOutput output;
	output.position = mul(input.position, SUNNY_ModelMatrix);
	output.positionCS =  mul(output.position, mul(SUNNY_ViewMatrix, SUNNY_ProjectionMatrix));
	output.normal = mul(input.normal, wsTransform);
	output.binormal = mul(input.binormal, wsTransform);
	output.tangent = mul(input.tangent, wsTransform);
	output.uv = input.uv;
	output.color = float3(1.0f, 1.0f, 1.0f);
	output.shadowCoord = float4(0.0f, 0.0f, 0.0f, 0.0f); // output.shadowCoord = mul(output.position, depthBias);

	output.lightPosition = mul(output.positionCS, SUNNY_LightViewMatrix);//추가
	output.lightPosition = mul(output.lightPosition, SUNNY_LightProjectionMatrix);//추가
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

Texture2D textures : register(t0);
Texture2D shadowMap: register(t1);
SamplerState samplers : register(s0);

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


float4 PSMain(in VSOutput input) : SV_TARGET
{
	float3 normal = normalize(input.normal);

	float4 texColor = (float4)SUNNY_Color;

	if (SUNNY_HasTexture >= 1)
	{
		texColor = textures.Sample(samplers, input.uv);
	}

	texColor.rgb *= texColor.rgb; // 더 진해진다.

	float3 finalColor = CalcAmbient(normal, texColor.rgb);
	finalColor += CalcDirectional(input.position, normal, texColor, input.cameraPosition);

	float2 shadowMapUV = input.lightPosition.xy / input.lightPosition.w;
	shadowMapUV.y = -shadowMapUV.y;
	shadowMapUV = shadowMapUV * 0.5 + 0.5;

	float shadowDepth = shadowMap.Sample(samplers, shadowMapUV).r;
	float currentDepth = input.position.z / input.position.w;

	if (currentDepth > shadowDepth + 0.000125f)
	{
		finalColor.rgb *= 0.5f;
	}

	return float4(finalColor, texColor.a);
}