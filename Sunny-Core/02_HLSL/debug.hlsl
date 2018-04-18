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






Texture2D    ShadowMap	: register(t3);
SamplerState Sampler	: register(s3);


float LinearizeDepth(float4 uv)
{
	float zNear = 0.1;    // TODO: Replace by the zNear of your perspective projection
	float zFar = 1000.0; // TODO: Replace by the zFar  of your perspective projection
	float depth = ShadowMap.Sample(Sampler, uv).x;
	return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}

float4 PSMain(VSOutput input) : SV_TARGET
{
	int3 sampleIndices = int3(input.position.xy, 0);

	float3 diffuse = ShadowMap.Sample(Sampler, sampleIndices).r;

	//return float4(diffuse.rrr, 1.0f);
	float c = LinearizeDepth(input.position);
	return float4(c, c, c, 1.0f);
}