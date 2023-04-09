cbuffer ColorBuffer : register (b0)
{
	float4 color[10];
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	nointerpolation uint instanceId : INST_ID;
};

float4 main(PS_INPUT input) : SV_Target0
{
	return color[input.instanceId];
}