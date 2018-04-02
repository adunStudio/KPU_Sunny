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
}