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
	int    tid : TEXTURE_CASE;
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
	output.positionCS =  mul(output.position, mul(SUNNY_ViewMatrix, SUNNY_ProjectionMatrix));
	output.normal = mul(input.normal, wsTransform);
	output.uv = input.uv;
	output.color = float3(1.0f, 1.0f, 1.0f);
	output.shadowCoord = float4(0.0f, 0.0f, 0.0f, 0.0f); // output.shadowCoord = mul(output.position, depthBias);
	output.tid = 0;

	output.cameraPosition = SUNNY_CameraPosition;

	return output;
}

struct PSOutput
{
	float4 diffuse  :  SV_TARGET0;
	float4 normal   :  SV_TARGET1;
	float4 position :  SV_TARGET2;
	float4 id       :  SV_TARGET3;
};

Texture2D textures[6] : register(t0);
SamplerState samplers : register(s0);

cbuffer PSSystemUniforms : register(b0)
{
	float4 SUNNY_Color;
	float4 SUNNY_ID;
	float  SUNNY_HasTexture;
};

PSOutput PSMain(in VSOutput input)
{
	float4 texColor = (float4)SUNNY_Color;

	if (SUNNY_HasTexture >= 1)
	{
		switch (input.tid)
		{
		case 0:
			texColor = textures[0].Sample(samplers, input.uv); break;
		case 1:
			texColor = textures[1].Sample(samplers, input.uv); break;
		case 2:
			texColor = textures[2].Sample(samplers, input.uv); break;
		case 3:
			texColor = textures[0].Sample(samplers, input.uv); break;
		case 4:
			texColor = textures[1].Sample(samplers, input.uv); break;
		case 5:
			texColor = textures[2].Sample(samplers, input.uv); break;
		}

	}

	PSOutput output;

	float4 id = (float4)SUNNY_ID;
	float s_id = id.r + id.g * 256 + id.b * 256 * 256;

	output.diffuse = texColor;
	output.normal = float4(input.normal, s_id);
	output.position = input.position;
	output.id      = id;

	return output;
}