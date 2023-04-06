#pragma once

#include "Shapes.h"

class Cube : public InstanceShape
{
private:
	struct GeomBuffer
	{
		DirectX::XMMATRIX modelMatrix;
		DirectX::XMMATRIX normalTransform;
	};

	struct TextureNum
	{
		DirectX::XMINT4 i;
	};

	struct Vertex
	{
		float x, y, z;
		float tx, ty, tz;
		float nx, ny, nz;
		float u, v;
	};
public:
	HRESULT CreateGeometry(ID3D11Device* m_pDevice) final;
	HRESULT CreateShaders(ID3D11Device* m_pDevice) final;
	HRESULT CreateTextures(ID3D11Device* m_pDevice) final;
	void Draw(const DirectX::XMMATRIX& vp,
		ID3D11DeviceContext* m_pDeviceContext) final;
private:
	std::vector<GeomBuffer> geomBuffers;
	std::vector<TextureNum> texNumBuffers;
	std::vector<float> rotateSpeed;
	std::vector<float> rotateAngle;
};