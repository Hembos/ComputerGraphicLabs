#pragma once

#include "../Shaders.h"

#include <d3d11.h>
#include <DirectXMath.h>

#include <vector>

#pragma comment(lib, "d3d11")

#define SAFE_RELEASE(p) if (p != NULL) { p->Release(); p = NULL; }

class InstanceShape
{
protected:
	struct SceneBuffer
	{
		DirectX::XMMATRIX vp;
		DirectX::XMVECTOR cameraPos;
	};
public:
	virtual HRESULT CreateGeometry(ID3D11Device* m_pDevice) = 0;
	virtual HRESULT CreateShaders(ID3D11Device* m_pDevice) = 0;
	virtual HRESULT CreateTextures(ID3D11Device* m_pDevice) = 0;
	virtual void Draw(const DirectX::XMMATRIX& projMatrix, const DirectX::XMMATRIX& viewMatrix,
						ID3D11DeviceContext* m_pDeviceContext) = 0;

	HRESULT setRasterizerState(ID3D11Device* m_pDevice, D3D11_CULL_MODE cullMode);
	void addInstance();

	void Translate(DirectX::XMMATRIX translateMatrix, int ind);
	void Scale(DirectX::XMMATRIX scaleMatrix, int ind);
	void Rotate(DirectX::XMMATRIX rotateMatrix, int ind);

	int getNumInstances();

	void Clean();
protected:
	ID3D11Buffer* m_pIndexBuffer = NULL;
	ID3D11Buffer* m_pVertextBuffer = NULL;

	ID3D11RasterizerState* rasterizerState;

	VertexShader vs;
	PixelShader ps;
	ID3D11InputLayout* m_pInputLayout = NULL;

	std::vector<ID3D11Buffer*> constBuffers;
	std::vector<ID3D11SamplerState*> samplers;
	std::vector<ID3D11ShaderResourceView*> resources;

	std::vector<DirectX::XMMATRIX> translateMatrices;
	std::vector<DirectX::XMMATRIX> scaleMatrices;
	std::vector<DirectX::XMMATRIX> rotateMatrices;

	SceneBuffer scBuffer;

	int maxInstancesNum = 100;
	int numInstances = 0;
};