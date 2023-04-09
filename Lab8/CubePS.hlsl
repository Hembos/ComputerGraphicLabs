#include "Light.hlsli"

Texture2D breaksTexture : TEXTURE: register(t0);
Texture2D catTexture : TEXTURE: register(t1);
Texture2D normalMapTexture : TEXTURE: register(t2);
SamplerState objSamplerState : SAMPLER: register(s0);

cbuffer TextureNumInst : register (b2)
{
	int4 textureNum[100];
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float3 tang : TANGENT;
	float3 normal : NORMAL;
	float4 worldPos : POSITION;
	float2 texCoord : TEXCOORD;
	nointerpolation uint instanceId : INST_ID;
};

float4 main(PS_INPUT input) : SV_Target0
{
	float4 pixelColor;
	float3 norm;
	if (textureNum[input.instanceId][0] == 0)
	{
		pixelColor = breaksTexture.Sample(objSamplerState, input.texCoord);

		float3 binorm = normalize(cross(input.normal, input.tang));
		float3 localNorm = normalMapTexture.Sample(objSamplerState, input.texCoord).xyz * 2.0 - float3(1.0, 1.0, 1.0);
		norm = localNorm.x * normalize(input.tang) + localNorm.y * binorm + localNorm.z * normalize(input.normal);
	}
	else
	{
		pixelColor = catTexture.Sample(objSamplerState, input.texCoord);
		norm = normalize(input.normal);
	}

	float3 lightColor = applyLight(norm, input.worldPos.xyz);
	float4 finalColor = float4(pixelColor.xyz * lightColor, pixelColor[3]);

	return finalColor;
}