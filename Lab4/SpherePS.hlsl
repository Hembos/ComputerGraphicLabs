TextureCube colorTexture : register(t0);
SamplerState colorSampler : register(s0);

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float3 localPos : POSITION1;
};

float4 main(PS_INPUT pixel) : SV_TARGET
{
	return float4(colorTexture.Sample(colorSampler, pixel.localPos).xyz, 1.0);
}
