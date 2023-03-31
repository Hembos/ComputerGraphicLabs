struct LightDesc
{
	float4 pos;
	float4 color;
};

cbuffer LightBuffer : register (b0)
{
	LightDesc lights[10];
	float4 ambient;
	int lightsCount;
	float diffuseCoef;
	float specularCoef;
	float shine;
};

float3 applyLight(in float3 norm, in float3 worldPos)
{
	float3 color = ambient.xyz * ambient[3];

	for (int i = 0; i < lightsCount; i++)
	{
		float3 lightDir = lights[i].pos.xyz - worldPos;
		float lightDist = length(lightDir);
		lightDir /= lightDist;

		float atten = clamp(1.0 / (lightDist * lightDist), 0, 1);

		color += max(dot(lightDir, norm), 0) * atten * lights[i].color.xyz * diffuseCoef;
	}

	return color;
}