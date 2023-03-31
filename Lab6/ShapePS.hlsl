#include "Light.hlsli"

#ifdef USE_TEXTURE
	Texture2D objTexture : TEXTURE: register(t0);
	SamplerState objSamplerState : SAMPLER: register(s0);
#else
	cbuffer ColorBuffer : register (b1)
	{
		float4 color;
	};
#endif

struct PS_INPUT
{
	float4 pos : SV_POSITION;
#ifdef USE_TEXTURE
	float2 texCoord : TEXCOORD;
#endif
};

float4 main(PS_INPUT input) : SV_Target0
{
#ifdef USE_TEXTURE
	float4 pixelColor = objTexture.Sample(objSamplerState, input.texCoord);
#else
	float4 pixelColor = color;
#endif

	float4 finalColor = pixelColor;
	
#ifdef APPLY_LIGHT
	float3 ambientColor = ambient.xyz * ambient[3];
	finalColor = float4(finalColor.xyz * ambientColor, finalColor[3]);
#endif

	return finalColor;
}