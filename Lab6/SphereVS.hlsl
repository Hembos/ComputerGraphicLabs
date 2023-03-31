#include "SceneBuffer.hlsli"

cbuffer GeomBuffer : register (b1)
{
	float4x4 model;
	float4 cameraPos;
	float4 radius;
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

	float3 pos = cameraPos.xyz + vertex.pos * radius.x;
	float4x4 world = mul(model, vp);
	result.pos = mul(float4(pos, 1.0f), world);
	result.pos.z = 0.0f;
	result.localPos = vertex.pos;

	return result;
}