cbuffer GeomBuffer : register (b0)
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
	output.pos = mul(float4(input.pos, 1.0f), model);
	output.texCoord = input.texCoord;
	return output;
}