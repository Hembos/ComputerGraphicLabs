cbuffer SceneBuffer : register (b0)
{
	float4x4 vp;
	float4 cameraPos;
	float4 frustum[6];
};
