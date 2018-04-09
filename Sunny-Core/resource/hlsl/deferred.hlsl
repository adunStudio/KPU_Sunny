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
	Light    SUNNY_Light;
	float3	 SUNNY_CameraPosition;
};

Texture2D positionGB	: register(t0);
Texture2D normalGB		: register(t1);
Texture2D diffuseGB		: register(t2);
SamplerState Sampler	: register(s0);

float4 PSMain(VSOutput input) : SV_TARGET
{
	int3 sampleIndices = int3(input.position.xy, 0);

	float3 normal = normalGB.Load(sampleIndices).xyz;

	float3 position = positionGB.Load(sampleIndices).xyz;

	float3 diffuse = diffuseGB.Load(sampleIndices).xyz;

	float3 L = SUNNY_Light.position;

	float lightAmountDL = saturate(dot(normal, L));

	float3 color = diffuse;// SUNNY_Light.color * lightAmountDL * diffuse;

	return float4(color, 1.0f);
}