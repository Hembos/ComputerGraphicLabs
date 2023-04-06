#pragma once

#include "Shapes.h"

class Sphere : public InstanceShape
{
private:
	struct GeomBuffer
	{
		DirectX::XMMATRIX modelMatrix;
	};

	struct ColorBuffer
	{
		DirectX::XMVECTOR color = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	};

	struct Vertex
	{
		float x, y, z;
	};
public:
	HRESULT CreateGeometry(ID3D11Device* m_pDevice) final;
	HRESULT CreateShaders(ID3D11Device* m_pDevice) final;
	HRESULT CreateTextures(ID3D11Device* m_pDevice) final;
	void Draw(const DirectX::XMMATRIX& vp,
		ID3D11DeviceContext* m_pDeviceContext) final;

	void setColor(const DirectX::XMVECTOR& color, int idx);
private:
	std::vector<GeomBuffer> geomBuffers;
	std::vector<ColorBuffer> colorBuffers;

	int numIndeces;
};