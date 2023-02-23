cbuffer GeomBuffer : register (b0)
{
	float4x4 model;
};

struct VSInput
{
	float3 pos : POSITION;
	float4 color : COLOR;
};

struct VSOutput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

VSOutput vs(VSInput vertex)
{
	VSOutput result;

	result.pos = mul(model, float4(vertex.pos, 1.0f));
	result.color = vertex.color;

	return result;
}
