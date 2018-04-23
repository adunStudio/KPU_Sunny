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


Texture2D diffuseGB		: register(t8);
Texture2D normalGB		: register(t9);
SamplerState Sampler	: register(s8);

float4 PSMain(VSOutput input) : SV_TARGET
{
	float4 center;
	float4 left, top, right, bottom;
	float4 left_top, left_bottom, right_top, right_bottom;

	center = normalGB.Load(float3(input.position.x, input.position.y, 0)).xyzw;

	left = normalGB.Load(float3(input.position.x + 0.5f, input.position.y, 0)).xyzw;
	if (center.a != left.a) return float4(0.1f, 0.1f, 0.1f, 0.89);

	right  = normalGB.Load(float3(input.position.x - 0.5f, input.position.y, 0)).xyzw;
	if (center.a != right.a) return float4(0.1f, 0.1f, 0.1f, 0.89);
	
	right_top = normalGB.Load(float3(input.position.x - 0.5f, input.position.y + 0.5f, 0)).xyzw;
	if (center.a != right_top.a) return float4(0.1f, 0.1f, 0.1f, 0.89);

	left_top = normalGB.Load(float3(input.position.x + 0.5f, input.position.y + 0.5f, 0)).xyzw;
	if (center.a != left_top.a) return float4(0.1f, 0.1f, 0.1f, 0.89);

	left_bottom = normalGB.Load(float3(input.position.x + 0.5f, input.position.y - 0.5f, 0)).xyzw;
	if (center.a != left_bottom.a) return float4(0.1f, 0.1f, 0.1f, 0.89);

	right_bottom = normalGB.Load(float3(input.position.x - 0.5f, input.position.y - 0.5f, 0)).xyzw;
	if (center.a != right_bottom.a) return float4(0.1f, 0.1f, 0.1f, 0.89);

	top = normalGB.Load(float3(input.position.x, input.position.y + 0.5f, 0)).xyzw;
	if (center.a != top.a) return float4(0.1f, 0.1f, 0.1f, 0.89);

	bottom = normalGB.Load(float3(input.position.x, input.position.y - 0.5f, 0)).xyzw;
	if (center.a != bottom.a) return float4(0.1f, 0.1f, 0.1f, 0.89);


	/*float3 dw = left.rgb + right.rgb - 2 * center.rgb;
	float3 dh = top.rgb  + bottom.rgb - 2 * center.rgb;

	float lw = length(dw);
	float lh = length(dh);

	if (sqrt(lw * lw -  lh * lh) > 0.05) return float4(0.1f, 0.1f, 0.1f, 0.89);*/
	//int3 sampleIndices = int3(input.position.xy, 0);
	//float3 diffuse = diffuseGB.Load(sampleIndices).xyz;
	
	return float4(1, 1, 1, 0);
}