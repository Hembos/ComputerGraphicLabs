#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>

#include <string>

#pragma comment(lib, "d3dcompiler")

#define SAFE_RELEASE(p) if (p != NULL) { p->Release(); p = NULL; }

class VertexShader
{
public:
	void Clean();
	bool Initialize(ID3D11Device* device, std::wstring shaderpath);
	ID3D11VertexShader* GetShader();
	ID3D10Blob* GetBuffer();
private:
	ID3D11VertexShader* shader;
	ID3D10Blob* shaderBuffer;
};

class PixelShader
{
public:
	void Clean();
	bool Initialize(ID3D11Device* device, std::wstring shaderpath);
	ID3D11PixelShader* GetShader();
	ID3D10Blob* GetBuffer();
private:
	ID3D11PixelShader* shader;
	ID3D10Blob* shaderBuffer;
};
