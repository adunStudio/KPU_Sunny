#pragma once

#include "../../sunny.h"

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



const std::string default3DShader_src = R"(
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
	float3 direction;
	float p1;
	float3 lightVector;
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

float4 PSMain(in VSOutput input) : SV_TARGET
{
	float4 texColor = (float4)SUNNY_Color;
	
	if(SUNNY_HasTexture >= 1)
	{
		texColor *=  textures.Sample(samplers, input.uv);
	}


	return texColor;
})";
	}
}