cbuffer VSSystemUniforms : register(b0)
{
	float4x4 SUNNY_ModelMatrix;
	float4x4 SUNNY_LightProjectionMatrix;
	float4x4 SUNNY_LightViewMatrix;
};

struct VSInput
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

struct VSOutput
{
	float4 position : SV_POSITION;
};

VSOutput VSMain(in VSInput input)
{
	VSOutput output;

	output.position = mul(input.position,  SUNNY_ModelMatrix);
	output.position = mul(output.position, SUNNY_LightViewMatrix);
	output.position = mul(output.position, SUNNY_LightProjectionMatrix);

	return output;
}


void PSMain(VSOutput input)
{
}