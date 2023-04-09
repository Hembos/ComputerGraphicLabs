struct PSInput
{
	float4 pos : SV_Position;
	float2 uv : TEXCOORD;
};

cbuffer PostProcBuffer : register (b0)
{
	float4 contrastAlpha;
	int4 postProcOper;
};

Texture2D colorTexture : register(t0);
SamplerState colorSampler : register(s0);

float4 main(PSInput input) : SV_Target0
{
	float3 color = colorTexture.Sample(colorSampler, input.uv).xyz;

	if (postProcOper[0] == 1)
	{
		color[0] = (color[0] * 0.393f) + (color[1] * 0.769f) + (color[2] * 0.189f);
		color[1] = (color[0] * 0.349f) + (color[1] * 0.686f) + (color[2] * 0.168f);
		color[2] = (color[0] * 0.272f) + (color[1] * 0.534f) + (color[2] * 0.131f);
	}

	if (postProcOper[1] == 1)
	{
		color[0] = (color[0] * contrastAlpha);
		color[1] = (color[1] * contrastAlpha);
		color[2] = (color[2] * contrastAlpha);
	}

	return float4(color, 1.0f);
}