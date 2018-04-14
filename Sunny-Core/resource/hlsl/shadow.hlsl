cbuffer VSSystemUniforms : register(b0)
{
	float4x4 SUNNY_ProjectionMatrix;
	float4x4 SUNNY_ViewMatrix;
	float4x4 SUNNY_ModelMatrix;
	float4x4 SUNNY_LightProjectionMatrix;
	float4x4 SUNNY_LightViewMatrix;
};

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
	float4 position : SV_POSITION;
	float4 lightViewPosition : TEXCOORD1;
};

VSOutput VSMain(in VSInput input)
{
	VSOutput output;

	output.lightViewPosition = mul(input.position, SUNNY_ModelMatrix);
	output.lightViewPosition = mul(input.position, mul(SUNNY_ViewMatrix, SUNNY_ProjectionMatrix));
	output.lightViewPosition = mul(output.lightViewPosition, SUNNY_LightViewMatrix);
	output.lightViewPosition = mul(output.lightViewPosition, SUNNY_LightProjectionMatrix);

	output.position = output.lightViewPosition;

	return output;
}


float4 PSMain(VSOutput Input) : SV_TARGET
{
	float depth = Input.lightViewPosition.z / Input.lightViewPosition.w;
	
	return float4(depth.xxx, 1);

	//return float4(depth.xxx, 1);
}