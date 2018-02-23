struct VSInput
{
	float4 position : POSITION;
	float2 uv : TEXCOORD;
	float2 mask_uv : MASKUV;
	float tid : ID;         
	float mid : MASKID;
	float4 color : COLOR;
};
struct VSOutput
{
	float4 positionCS : SV_POSITION;
	float4 position : POSITION;
	float2 uv : TEXCOORD;
	float2 mask_uv : MASKUV;
	float tid : ID;
	float mid : MASKID;
	float4 color : COLOR;
};
cbuffer VSUniforms : register(b0)
{
	float4x4 SUNNY_ProjectionMatrix;
	float4x4 SUNNY_ViewMatrix;
	float4x4 SUNNY_MaskMatrix;
}
cbuffer VSa : register(b1)
{
	float4x4 a;
}
VSOutput VSMain(VSInput input)
{
	VSOutput output;
	output.positionCS = mul(input.position, SUNNY_ProjectionMatrix);
	output.position = input.position;
	output.uv = input.uv;
	output.tid = input.tid;
	output.mid = input.mid;
	output.color = input.color;
	output.mask_uv = input.mask_uv;
	return output;
}
Texture2D textures[32] : register(t0);
SamplerState samplers[32] : register(s0);
float4 PSMain(VSOutput input) : SV_TARGET
{
	float4 texColor = input.color;
	float4 maskColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	switch (input.tid)
	{
	case 1:
		texColor *= textures[0].Sample(samplers[0], input.uv);
		break;
	case 2:
		texColor *= textures[1].Sample(samplers[1], input.uv);
		break;
	case 3:
		texColor *= textures[2].Sample(samplers[2], input.uv);
		break;
	case 4:
		texColor *= textures[3].Sample(samplers[3], input.uv);
		break;
	case 5:
		texColor *= textures[4].Sample(samplers[4], input.uv);
		break;
	case 6:
		texColor *= textures[5].Sample(samplers[5], input.uv);
		break;
	case 7:
		texColor *= textures[6].Sample(samplers[6], input.uv);
		break;
	case 8:
		texColor *= textures[7].Sample(samplers[7], input.uv);
		break;
	case 9:
		texColor *= textures[8].Sample(samplers[8], input.uv);
		break;
	case 10:
		texColor *= textures[9].Sample(samplers[9], input.uv);
		break;
	case 11:
		texColor *= textures[10].Sample(samplers[10], input.uv);
		break;
	case 12:
		texColor *= textures[11].Sample(samplers[11], input.uv);
		break;
	case 13:
		texColor *= textures[12].Sample(samplers[12], input.uv);
		break;
	case 14:
		texColor *= textures[13].Sample(samplers[13], input.uv);
		break;
	case 15:
		texColor *= textures[14].Sample(samplers[14], input.uv);
		break;
	case 16:
		texColor *= textures[15].Sample(samplers[15], input.uv);
		break;
	}
	switch (input.mid)
	{
	case 1:
		maskColor *= textures[0].Sample(samplers[0], input.mask_uv);
		break;
	case 2:
		maskColor *= textures[1].Sample(samplers[1], input.mask_uv);
		break;
	case 3:
		maskColor *= textures[2].Sample(samplers[2], input.mask_uv);
		break;
	case 4:
		maskColor *= textures[3].Sample(samplers[3], input.mask_uv);
		break;
	case 5:
		maskColor *= textures[4].Sample(samplers[4], input.mask_uv);
		break;
	case 6:
		maskColor *= textures[5].Sample(samplers[5], input.mask_uv);
		break;
	case 7:
		maskColor *= textures[6].Sample(samplers[6], input.mask_uv);
		break;
	case 8:
		maskColor *= textures[7].Sample(samplers[7], input.mask_uv);
		break;
	case 9:
		maskColor *= textures[8].Sample(samplers[8], input.mask_uv);
		break;
	case 10:
		maskColor *= textures[9].Sample(samplers[9], input.mask_uv);
		break;
	case 11:
		maskColor *= textures[10].Sample(samplers[10], input.mask_uv);
		break;
	case 12:
		maskColor *= textures[11].Sample(samplers[11], input.mask_uv);
		break;
	case 13:
		maskColor *= textures[12].Sample(samplers[12], input.mask_uv);
		break;
	case 14:
		maskColor *= textures[13].Sample(samplers[13], input.mask_uv);
		break;
	case 15:
		maskColor *= textures[14].Sample(samplers[14], input.mask_uv);
		break;
	case 16:
		maskColor *= textures[15].Sample(samplers[15], input.mask_uv);
		break;
	}
	return texColor * maskColor;
};