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
#ifdef APPLY_LIGHT
	float3 normal : NORMAL;
	float4 worldPos : POSITION;
#endif
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
	float3 norm = normalize(input.normal);
	float3 lightColor = applyLight(norm, input.worldPos.xyz);
	finalColor = float4(finalColor.xyz * lightColor, finalColor[3]);
	/*float3 ambientColor = ambient.xyz * ambient[3];
	finalColor = float4(finalColor.xyz * ambientColor, finalColor[3]);
	float3 norm = normalize(input.normal);

	for (int i = 0; i < lightsCount; i++)
	{
		float3 lightDir = lights[i].pos.xyz - input.worldPos.xyz;
		float lightDist = length(lightDir);
		lightDir /= lightDist;

		float atten = clamp(1.0 / (lightDist * lightDist), 0, 1);

		finalColor += float4(pixelColor * max(dot(lightDir, norm), 0) * atten * lights[i].color.xyz * diffusionCoef, finalColor[3]);
	}*/

#endif

	return finalColor;
}