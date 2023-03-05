#include "Shaders.h"

void VertexShader::Clean()
{
	SAFE_RELEASE(shader);
	SAFE_RELEASE(shaderBuffer);
}

bool VertexShader::Initialize(ID3D11Device* device, std::wstring shaderpath)
{
	HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), &shaderBuffer);
	if (!SUCCEEDED(hr))
	{
		return false;
	}

	hr = device->CreateVertexShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &shader);
	if (!SUCCEEDED(hr))
	{
		return false;
	}

	return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
	return shader;
}

ID3D10Blob* VertexShader::GetBuffer()
{
	return shaderBuffer;
}

void PixelShader::Clean()
{
	SAFE_RELEASE(shader);
	SAFE_RELEASE(shaderBuffer);
}

bool PixelShader::Initialize(ID3D11Device* device, std::wstring shaderpath)
{
	HRESULT hr = D3DReadFileToBlob(shaderpath.c_str(), &shaderBuffer);
	if (!SUCCEEDED(hr))
	{
		return false;
	}

	hr = device->CreatePixelShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &shader);
	if (!SUCCEEDED(hr))
	{
		return false;
	}

	return true;
}

ID3D11PixelShader* PixelShader::GetShader()
{
	return shader;
}

ID3D10Blob* PixelShader::GetBuffer()
{
	return shaderBuffer;
}