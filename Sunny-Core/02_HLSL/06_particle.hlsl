struct VSInput
{
	float4 position : POSITION;
	float4 color    : COLOR;
	float2 uv       : TEXCOORD;
};

struct VSOutput
{
	float4 position       : SV_POSITION;
	float2 uv             : TEXCOORD0;
	float4 color          : COLOR;
};

cbuffer VSSystemUniforms : register(b0)
{
	float4x4 SUNNY_ProjectionMatrix;
	float4x4 SUNNY_ViewMatrix;
	float4x4 SUNNY_ModelMatrix;
};

VSOutput VSMain(in VSInput input)
{
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.


	float3x3 wsTransform = (float3x3)SUNNY_ModelMatrix;

	VSOutput output;
	output.position = mul(input.position, SUNNY_ModelMatrix);
	output.position = mul(output.position, SUNNY_ViewMatrix);
	output.position = mul(output.position, SUNNY_ModelMatrix);
	//output.position       = mul(input.position, SUNNY_ModelMatrix);
	//output.position       = mul(output.position, mul(SUNNY_ViewMatrix, SUNNY_ProjectionMatrix));
	output.uv             = input.uv;
	output.color          = input.color;
	
	return output;
}

cbuffer PSSystemUniforms : register(b0)
{
	float4 SUNNY_Color;
	float  SUNNY_HasTexture;
};


Texture2D particleTexture    : register(t0);
SamplerState particleSampler : register(s0);


float4 PSMain(in VSOutput input) : SV_TARGET
{
	return input.color;
	float4 textureColor;
	float4 finalColor;

	textureColor = particleTexture.Sample(particleSampler, input.uv);
	finalColor = textureColor * input.color;

	return finalColor;
}

