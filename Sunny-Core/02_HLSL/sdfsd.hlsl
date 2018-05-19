struct VSInput
{
	float4 position : POSITION;
	float3 normal   : NORMAL;
	float2 uv       : TEXCOORD;
	float  tid : TID;
};

struct VSOutput
{
	float4 positionCS     : SV_POSITION;
	float4 position       : POSITION;
	float3 normal         : NORMAL;
	float2 uv             : TEXCOORD0;
	float  tid : TID;
	float3 diffuse        : TEXCOORD1;
	float3 viewDir        : TEXCOORD2;
	float3 reflection     : TEXCOORD3;
};

cbuffer VSSystemUniforms : register(b0)
{
	float4x4 SUNNY_ProjectionMatrix;
	float4x4 SUNNY_ViewMatrix;
	float4x4 SUNNY_ModelMatrix;
	float4x4 SUNNY_LightProjectionMatrix;
	float4x4 SUNNY_LightViewMatrix;
	float3	 SUNNY_CameraPosition;
};

VSOutput VSMain(in VSInput input)
{
	float3x3 wsTransform = (float3x3)SUNNY_ModelMatrix;

	VSOutput output;

	output.position = mul(input.position, SUNNY_ModelMatrix);
	output.positionCS = mul(output.position, mul(SUNNY_ViewMatrix, SUNNY_ProjectionMatrix));
	output.normal = mul(input.normal, wsTransform);
	output.uv = input.uv;
	output.tid = input.tid;

	float3 lightPosition = float3(0, 3000, 0);

	float3 lightDir = output.position.xyz - lightPosition.xyz;
	float3 viewDir = output.position.xyz - lightPosition.xyz;

	lightDir = normalize(lightDir);
	output.viewDir = normalize(viewDir);


	output.diffuse = dot(-lightDir, input.normal);
	output.reflection = reflect(lightDir, input.normal);

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
	Light SUNNY_Light;
	float4 SUNNY_Color;
	float  SUNNY_HasTexture;
};

Texture2D textures[6] : register(t0);
Texture2D diffuseMap  : register(t7);
Texture2D normalMap   : register(t8);
Texture2D positionMap : register(t9);
Texture2D depthMap    : register(t10);
Texture2D idMap       : register(t11);
Texture2D shadowMap   : register(t12);


SamplerState samplers        : register(s0);
SamplerState geometrySampler : register(s1);
SamplerState shadowSampler   : register(s2);

float cellsmooth3(float x)
{
	return 0.5*(smoothstep(0.16, 0.34, x) * smoothstep(0.66, 0.84, x));
}

float4 PSMain(in VSOutput input) : SV_TARGET
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
			texColor = textures[3].Sample(samplers, input.uv); break;
		case 4:
			texColor = textures[4].Sample(samplers, input.uv); break;
		case 5:
			texColor = textures[5].Sample(samplers, input.uv); break;
		}

	}

	float3 diffuse = saturate(input.diffuse);


	float3 viewDir = normalize(input.viewDir);
	float3 reflection = normalize(input.reflection);

	float3 specular = 0;

	if (diffuse.x > 0)
	{
		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 10.0f);
	}

	float3 ambient = float3(0.1f, 0.1f, 0.1f);

	return float4(texColor * diffuse + specular , 1);

}

