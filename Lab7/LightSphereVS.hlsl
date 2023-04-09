#include "Scene.hlsli"

struct GeomBuffer
{
	float4x4 model;
};

cbuffer GeomBufferInst : register (b1)
{
	GeomBuffer geomBuffer[10];
};

cbuffer TextureNumInst : register (b2)
{
	int4 visibleInd[10];
};

struct VS_INPUT
{
	float3 pos : POSITION;
	nointerpolation uint instanceId : SV_InstanceID;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	nointerpolation uint instanceId : INST_ID;
};

VS_OUTPUT main(VS_INPUT input)
{
	unsigned int idx = visibleInd[input.instanceId];

	VS_OUTPUT output;
	float4 world = mul(float4(input.pos, 1.0f), geomBuffer[idx].model);
	output.pos = mul(world, vp);
	output.instanceId = idx;
	return output;
}