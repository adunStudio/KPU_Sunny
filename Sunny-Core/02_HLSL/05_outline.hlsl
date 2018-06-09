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
	output.position.z = 0;
	output.position.w = 1;

	return output;
}

Texture2D diffuseMap  : register(t7);
Texture2D normalMap   : register(t8);
Texture2D positionMap : register(t9);
Texture2D depthMap    : register(t10);
Texture2D shadowMap   : register(t12);

SamplerState geometrySampler : register(s1);

int CheckOutLine(VSOutput input)
{
	float4 center;
	float4 left, top, right, bottom;
	float4 left_top, left_bottom, right_top, right_bottom;

	center = normalMap.Load(float3(input.position.x, input.position.y, 0)).xyzw;

	left = normalMap.Load(float3(input.position.x + 0.5f, input.position.y, 0)).xyzw;
	if (center.a != left.a) return 1;

	right = normalMap.Load(float3(input.position.x - 0.5f, input.position.y, 0)).xyzw;
	if (center.a != right.a) return 1;

	right_top = normalMap.Load(float3(input.position.x - 0.5f, input.position.y + 0.5f, 0)).xyzw;
	if (center.a != right_top.a) return 1;

	left_top = normalMap.Load(float3(input.position.x + 0.5f, input.position.y + 0.5f, 0)).xyzw;
	if (center.a != left_top.a) return 1;

	left_bottom = normalMap.Load(float3(input.position.x + 0.5f, input.position.y - 0.5f, 0)).xyzw;
	if (center.a != left_bottom.a) return 1;

	right_bottom = normalMap.Load(float3(input.position.x - 0.5f, input.position.y - 0.5f, 0)).xyzw;
	if (center.a != right_bottom.a) return 1;

	top = normalMap.Load(float3(input.position.x, input.position.y + 0.5f, 0)).xyzw;
	if (center.a != top.a) return 1;

	bottom = normalMap.Load(float3(input.position.x, input.position.y - 0.5f, 0)).xyzw;
	if (center.a != bottom.a) return 1;

	return 0;
}


cbuffer PSSystemUniforms : register(b0)
{
	float4x4 SUNNY_ProjectionMatrix;
};



float4 PSMain(VSOutput input) : SV_TARGET
{
	//return diffuseMap.Load(float3(input.position.x, input.position.y, 0)).xyzw;

	if ( CheckOutLine(input) > 0 )
		return float4(0.1f, 0.1f, 0.1f, 0.89f);

	return float4(0, 0, 0, 0);
}