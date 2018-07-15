struct VSInput
{
	float4 position : POSITION;
	float3 normal   : NORMAL;
	float2 uv       : TEXCOORD;
	float  tid      : TID;
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
	int    tid : TEXTURE_CASE;
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

cbuffer VSUniforms : register(b1)
{
	float4x4 invViewProjMatrix;
}

VSOutput VSMain(in VSInput input)
{
	VSOutput output;

	float4 pos = input.position;
	pos.z = 1;

	output.positionCS = pos;
	output.position = mul(pos, invViewProjMatrix);
	output.uv = input.uv;
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



TextureCube u_EnvironmentMap : register(t0);
SamplerState u_EnvironmentMapSampler : register(s0);

float4 PSMain(in VSOutput input) : SV_TARGET
{
	return u_EnvironmentMap.SampleLevel(u_EnvironmentMapSampler, input.position, 4);
}

