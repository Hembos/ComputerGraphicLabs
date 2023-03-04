cbuffer GeomBuffer : register (b0)
{
	float4x4 model;
	float4 cameraPos;
};

cbuffer SceneBuffer : register (b1)
{
	float4x4 vp;
};

struct VSInput
{
	float3 pos : POSITION;
};

struct VSOutput
{
	float4 pos : SV_POSITION;
	float3 localPos : POSITION1;
};

VSOutput vs(VSInput vertex)
{
	VSOutput result;

	float3 pos = cameraPos.xyz + vertex.pos;

	float4 worldPos = mul(model, float4(pos, 1.0f));
	result.pos = mul(vp, worldPos);
	result.localPos = vertex.pos;

	return result;
}
