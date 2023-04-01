#pragma once

#include "Shaders.h"

#include <d3d11.h>
#include <DirectXMath.h>

#include <vector>

#pragma comment(lib, "d3d11")

#define SAFE_RELEASE(p) if (p != NULL) { p->Release(); p = NULL; }

class Shape
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
	virtual void Draw(const DirectX::XMMATRIX& vp,
						ID3D11DeviceContext* m_pDeviceContext) = 0;

	virtual HRESULT setRasterizerState(ID3D11Device* m_pDevice, D3D11_CULL_MODE cullMode);
	virtual void addInstance() {};

	void Translate(DirectX::XMMATRIX translateMatrix, int ind);
	void Scale(DirectX::XMMATRIX scaleMatrix, int ind);
	void Rotate(DirectX::XMMATRIX rotateMatrix, int ind);

	void setMaxInstanceNum(int num);
	int getMaxInstanceNum();

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

class Cube : public Shape
{
	struct GeomBuffer
	{
		DirectX::XMMATRIX modelMatrix;
		//DirectX::XMMATRIX normalTransform;
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

	void addInstance() final;
private:
	GeomBuffer geomBuffers[100];
	std::vector<float> rotateSpeed;
	std::vector<float> rotate;
};

class Square : public Shape
{
	struct GeomBuffer
	{
		DirectX::XMMATRIX modelMatrix;
	};

	struct ColorBuffer
	{
		DirectX::XMVECTOR color;
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

	void setColor(DirectX::XMVECTOR color);

private:
	GeomBuffer geomBuffer;
	ColorBuffer colorBuffer;
};

class Sphere : public Shape
{
	struct GeomBuffer
	{
		DirectX::XMMATRIX modelMatrix;
	};

	struct ColorBuffer
	{
		DirectX::XMVECTOR color;
	};

	struct Vertex
	{
		float x, y, z;
		float u, v;
	};
public:
	HRESULT CreateGeometry(ID3D11Device* m_pDevice) final;
	HRESULT CreateShaders(ID3D11Device* m_pDevice) final;
	HRESULT CreateTextures(ID3D11Device* m_pDevice) final;
	void Draw(const DirectX::XMMATRIX& vp,
		ID3D11DeviceContext* m_pDeviceContext) final;

	void setColor(DirectX::XMVECTOR color);

private:
	GeomBuffer geomBuffer;
	ColorBuffer colorBuffer;
	int numIndeces;
};