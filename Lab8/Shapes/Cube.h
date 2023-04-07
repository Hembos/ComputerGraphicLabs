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

	struct VisibleIndexes
	{
		DirectX::XMINT4 idx;
	};

	struct TextureNum
	{
		DirectX::XMINT4 i;
	}; 

	struct CullParams
	{
		DirectX::XMINT4 numShapes;
		DirectX::XMFLOAT4 bbMin[100];
		DirectX::XMFLOAT4 bbMax[100];
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
	void Draw(const DirectX::XMMATRIX& projMatrix, const DirectX::XMMATRIX& viewMatrix,
		ID3D11DeviceContext* m_pDeviceContext) final;
	void RenderImGUI();

private:
	std::vector<GeomBuffer> geomBuffers;
	std::vector<VisibleIndexes> visibleIndBuffer;
	std::vector<TextureNum> texNumBuffers;
	std::vector<float> rotateSpeed;
	std::vector<float> rotateAngle;
	int visibleObjectNum = 0;

	CullParams clParams;
};