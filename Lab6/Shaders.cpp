#include "Shaders.h"

#include <fstream>
#include <vector>

void VertexShader::Clean()
{
	SAFE_RELEASE(shader);
	SAFE_RELEASE(shaderBuffer);
}

bool VertexShader::Initialize(ID3D11Device* device, std::wstring shaderpath)
{
	ShaderInclude include;
	ID3DBlob* pErrMsg = nullptr;
	HRESULT hr = D3DCompileFromFile(shaderpath.c_str(), NULL, &include, "main", "vs_5_0", NULL, NULL, &shaderBuffer, &pErrMsg);
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
	ShaderInclude include;
	ID3DBlob* pErrMsg = nullptr;
	HRESULT hr = D3DCompileFromFile(shaderpath.c_str(), NULL, &include, "main", "ps_5_0", NULL, NULL, &shaderBuffer, &pErrMsg);
	if (!SUCCEEDED(hr) && pErrMsg != nullptr)
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

HRESULT ShaderInclude::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes)
{
	std::wstring filePath(pFileName, pFileName + strlen(pFileName));
	if (IncludeType != D3D_INCLUDE_LOCAL)
		return E_FAIL;

	std::ifstream file(filePath, std::ios::in | std::ios::binary);

	std::vector<char> buffer;

	file.seekg(0, file.end);
	size_t length = file.tellg();
	file.seekg(0, file.beg);

	if (length > 0) 
	{
		buffer.resize(length);
		file.read(&buffer[0], length);
	}

	*pBytes = length;
	*ppData = static_cast<void*>(buffer.data());
	return S_OK;
}

HRESULT ShaderInclude::Close(LPCVOID pData)
{
	std::free(const_cast<void*>(pData));
	return S_OK;
}
