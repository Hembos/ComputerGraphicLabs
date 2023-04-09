#pragma once

#include "Shapes/Shapes.h"

class SkyBox
{
	struct SceneBuffer
	{
		DirectX::XMMATRIX vp;
		DirectX::XMVECTOR cameraPos;
	};
	struct GeomBuffer
	{
		DirectX::XMMATRIX modelMatrix;
		DirectX::XMVECTOR radius;
	};

	struct Vertex
	{
		float x, y, z;
	};
public:
	HRESULT CreateGeometry(ID3D11Device* m_pDevice);
	HRESULT CreateShaders(ID3D11Device* m_pDevice);
	HRESULT CreateTextures(ID3D11Device* m_pDevice);
	void Draw(const DirectX::XMMATRIX& vp,
		ID3D11DeviceContext* m_pDeviceContext);

	void setCamPos(DirectX::XMVECTOR camPos);
	void setRadius(const float& fov, const float& nearPlane, const float& width, const float& height);

	HRESULT setRasterizerState(ID3D11Device* m_pDevice, D3D11_CULL_MODE cullMode);

	void Clean();

private:
	ID3D11Buffer* m_pIndexBuffer = NULL;
	ID3D11Buffer* m_pVertextBuffer = NULL;
	std::vector<ID3D11Buffer*> constBuffers;
	std::vector<ID3D11SamplerState*> samplers;
	std::vector<ID3D11ShaderResourceView*> resources;

	VertexShader vs;
	PixelShader ps;
	ID3D11InputLayout* m_pInputLayout = NULL;

	ID3D11RasterizerState* rasterizerState;

	DirectX::XMMATRIX model = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX translateMatrix = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	DirectX::XMMATRIX rotateMatrix = DirectX::XMMatrixRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), 0.0f);

	SceneBuffer scBuffer;

	GeomBuffer geomBuffer;
	int numIndeces;
	DirectX::XMVECTOR camPos;
	float radius;
};