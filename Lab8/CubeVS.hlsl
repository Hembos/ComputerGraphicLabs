#include "Scene.hlsli"

struct GeomBuffer
{
	float4x4 model;
	float4x4 normalTransform;
};

cbuffer GeomBufferInst : register (b1)
{
	GeomBuffer geomBuffer[100];
};

cbuffer VisibleInd : register (b2)
{
	uint4 visibleInd[100];
};

struct VS_INPUT
{
	float3 pos : POSITION;
	float3 tang : TANGENT;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	nointerpolation uint instanceId : SV_InstanceID;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 tang : TANGENT;
	float3 normal : NORMAL;
	float4 worldPos : POSITION;
	float2 texCoord : TEXCOORD;
	nointerpolation uint instanceId : INST_ID;
};

VS_OUTPUT main(VS_INPUT input)
{
	unsigned int idx = visibleInd[input.instanceId];

	VS_OUTPUT output;
	float4 world = mul(float4(input.pos, 1.0f), geomBuffer[idx].model);
	output.pos = mul(world, vp);
	output.tang = mul(geomBuffer[idx].normalTransform, float4(input.tang, 0.0)).xyz;
	output.normal = mul(geomBuffer[idx].normalTransform, float4(input.normal, 0.0)).xyz;
	output.worldPos = world;
	output.texCoord = input.texCoord;
	output.instanceId = idx;
	return output;
}