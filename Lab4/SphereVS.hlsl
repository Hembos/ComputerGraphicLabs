cbuffer GeomBuffer : register (b0)
{
	float4x4 model;
	float4 cameraPos;
};

struct VS_INPUT
{
	float3 pos : POSITION;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 localPos : POSITION1;
};

VS_OUTPUT main(VS_INPUT vertex)
{
	VS_OUTPUT result;

	float3 pos = cameraPos.xyz + vertex.pos;

	result.pos = mul(float4(pos, 1.0f), model);
	result.localPos = vertex.pos;

	return result;
}