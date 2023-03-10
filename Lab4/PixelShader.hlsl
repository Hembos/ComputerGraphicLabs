Texture2D objTexture : TEXTURE: register(t0);
SamplerState objSamplerState : SAMPLER: register(s0);

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	float3 pixelColor = objTexture.Sample(objSamplerState, input.texCoord);
	return float4(pixelColor, 1.0f);
}