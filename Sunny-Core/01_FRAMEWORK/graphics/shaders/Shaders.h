#pragma once

#include "../../include.h"

namespace sunny
{
	namespace graphics
	{
		const std::string default2DShader_src = R"(
struct VSInput
{
	float4 position : POSITION;
	float2 uv : TEXCOORD;
	float2 mask_uv : MASKUV;
	float tid : ID;         
	float mid : MASKID;
	float4 color : COLOR;
};
struct VSOutput
{
	float4 positionCS : SV_POSITION;
	float4 position : POSITION;
	float2 uv : TEXCOORD;
	float2 mask_uv : MASKUV;
	float tid : ID;
	float mid : MASKID;
	float4 color : COLOR;
};
cbuffer VSUniforms : register(b0)
{
	float4x4 SUNNY_ProjectionMatrix;
	float4x4 SUNNY_ViewMatrix;
	float4x4 SUNNY_MaskMatrix;
}
VSOutput VSMain(VSInput input)
{
	VSOutput output;
	output.positionCS = mul(input.position, SUNNY_ProjectionMatrix);
	output.position = input.position;
	output.uv = input.uv;
	output.tid = input.tid;
	output.mid = input.mid;
	output.color = input.color;
	output.mask_uv = input.mask_uv;
	return output;
}
Texture2D textures[32] : register(t0);
SamplerState samplers[32] : register(s0);
float4 PSMain(VSOutput input) : SV_TARGET
{
	float4 texColor = input.color;
	float4 maskColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	switch (input.tid)
	{
	case 1:
		texColor *= textures[0].Sample(samplers[0], input.uv);
		break;
	case 2:
		texColor *= textures[1].Sample(samplers[1], input.uv);
		break;
	case 3:
		texColor *= textures[2].Sample(samplers[2], input.uv);
		break;
	case 4:
		texColor *= textures[3].Sample(samplers[3], input.uv);
		break;
	case 5:
		texColor *= textures[4].Sample(samplers[4], input.uv);
		break;
	case 6:
		texColor *= textures[5].Sample(samplers[5], input.uv);
		break;
	case 7:
		texColor *= textures[6].Sample(samplers[6], input.uv);
		break;
	case 8:
		texColor *= textures[7].Sample(samplers[7], input.uv);
		break;
	case 9:
		texColor *= textures[8].Sample(samplers[8], input.uv);
		break;
	case 10:
		texColor *= textures[9].Sample(samplers[9], input.uv);
		break;
	case 11:
		texColor *= textures[10].Sample(samplers[10], input.uv);
		break;
	case 12:
		texColor *= textures[11].Sample(samplers[11], input.uv);
		break;
	case 13:
		texColor *= textures[12].Sample(samplers[12], input.uv);
		break;
	case 14:
		texColor *= textures[13].Sample(samplers[13], input.uv);
		break;
	case 15:
		texColor *= textures[14].Sample(samplers[14], input.uv);
		break;
	case 16:
		texColor *= textures[15].Sample(samplers[15], input.uv);
		break;
	}
	switch (input.mid)
	{
	case 1:
		maskColor *= textures[0].Sample(samplers[0], input.mask_uv);
		break;
	case 2:
		maskColor *= textures[1].Sample(samplers[1], input.mask_uv);
		break;
	case 3:
		maskColor *= textures[2].Sample(samplers[2], input.mask_uv);
		break;
	case 4:
		maskColor *= textures[3].Sample(samplers[3], input.mask_uv);
		break;
	case 5:
		maskColor *= textures[4].Sample(samplers[4], input.mask_uv);
		break;
	case 6:
		maskColor *= textures[5].Sample(samplers[5], input.mask_uv);
		break;
	case 7:
		maskColor *= textures[6].Sample(samplers[6], input.mask_uv);
		break;
	case 8:
		maskColor *= textures[7].Sample(samplers[7], input.mask_uv);
		break;
	case 9:
		maskColor *= textures[8].Sample(samplers[8], input.mask_uv);
		break;
	case 10:
		maskColor *= textures[9].Sample(samplers[9], input.mask_uv);
		break;
	case 11:
		maskColor *= textures[10].Sample(samplers[10], input.mask_uv);
		break;
	case 12:
		maskColor *= textures[11].Sample(samplers[11], input.mask_uv);
		break;
	case 13:
		maskColor *= textures[12].Sample(samplers[12], input.mask_uv);
		break;
	case 14:
		maskColor *= textures[13].Sample(samplers[13], input.mask_uv);
		break;
	case 15:
		maskColor *= textures[14].Sample(samplers[14], input.mask_uv);
		break;
	case 16:
		maskColor *= textures[15].Sample(samplers[15], input.mask_uv);
		break;
	}
	return texColor * maskColor;
};)";



		const std::string default3DForwardShader_src = R"(
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
};

cbuffer VSSystemUniforms : register(b0)
{
	float4x4 SUNNY_ProjectionMatrix;
	float4x4 SUNNY_ViewMatrix;
	float4x4 SUNNY_ModelMatrix;
	float3	 SUNNY_CameraPosition;
	// float4x4 SUNNY_DepthBiasMatrix;
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
	
	if(SUNNY_HasTexture >= 1)
	{
		texColor *=  textures.Sample(samplers, input.uv);
	}

	texColor.rgb *= texColor.rgb; // 더 진해진다.

	float3 finalColor = CalcAmbient(normal, texColor.rgb);
	finalColor += CalcDirectional(input.position, normal, texColor, input.cameraPosition);

	return float4(finalColor, texColor.a);

	//return texColor;
})";

const std::string default3DDeferredShader_src = R"(
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
};

cbuffer VSSystemUniforms : register(b0)
{
	float4x4 SUNNY_ProjectionMatrix;
	float4x4 SUNNY_ViewMatrix;
	float4x4 SUNNY_ModelMatrix;
	float3	 SUNNY_CameraPosition;
	// float4x4 SUNNY_DepthBiasMatrix;
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

	output.cameraPosition = SUNNY_CameraPosition;

	return output;
}

struct PSOutput
{
	float4 position: SV_TARGET0;
	float4 diffuse:  SV_TARGET1;
	float4 normal:   SV_TARGET2;
};

Texture2D textures : register(t0);
SamplerState samplers : register(s0);

cbuffer PSSystemUniforms : register(b0)
{
	float4 SUNNY_Color;
	float  SUNNY_HasTexture;
};

PSOutput PSMain(in VSOutput input)
{
	PSOutput output;
	
	float4 texColor = (float4)SUNNY_Color;
	if (SUNNY_HasTexture >= 1)
	{
		texColor *= textures.Sample(samplers, input.uv);
	}

	output.position = input.position;
	output.diffuse = texColor;
	output.normal = float4(normalize(input.normal), 0);

	return output;
})";

const std::string default3DLightShader_src = R"(
struct VSInput
{
	float3 position : POSITION;
};

struct VSOutput
{
	float4 position : SV_POSITION;
};


VSOutput VSMain(uint id : SV_VertexID)
{
	VSOutput output;

	// Calculate the UV (0,0) to (2,2) via the ID
	float2 uv = float2(
		(id << 1) & 2,  // id % 2 * 2
		id & 2);

	// Adjust the position based on the UV
	output.position = float4(uv, 0, 1);
	output.position.x = output.position.x * 2 - 1;
	output.position.y = output.position.y * -2 + 1;

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

cbuffer PSSystemUniforms : register(b0)
{
	Light SUNNY_Light;
};

Texture2D positionGB	: register(t0);
Texture2D diffuseGB		: register(t1);
Texture2D normalGB		: register(t2);
SamplerState Sampler	: register(s0);

float4 PSMain(VSOutput input) : SV_TARGET
{
	int3 sampleIndices = int3(input.position.xy, 0);

	float3 normal = normalGB.Load(sampleIndices).xyz;

	float3 position = positionGB.Load(sampleIndices).xyz;

	float3 diffuse = diffuseGB.Load(sampleIndices).xyz;

	float3 L = -SUNNY_Light.position;

	float lightAmountDL = saturate(dot(normal, L));
	float3 color =  diffuse;// SUNNY_Light.color * SUNNY_Light.ambientDown * diffuse;

	return float4(color, 1.0f);
	//return float4(1.0f, 1.0f, 0, 1.0f);
})";
	}
}