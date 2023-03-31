#include "SceneBuffer.hlsli"

cbuffer GeomBuffer : register (b1)
{
	float4x4 model;
};

struct VS_INPUT
{
	float3 pos : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	float4x4 world = mul(model, vp);
	output.pos = mul(float4(input.pos, 1.0f), world);
	output.texCoord = input.texCoord;
	return output;
}