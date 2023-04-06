#include "Cube.h"
#include "../DDSTextureLoader11.h"

HRESULT Cube::CreateGeometry(ID3D11Device* m_pDevice)
{
    static const Vertex v[] = {
        {-0.5, -0.5,  0.5, 1, 0, 0,  0.0f,  -1.0f, 0.0f, 0, 1},
        { 0.5, -0.5,  0.5, 1, 0, 0, 0.0f,  -1.0f, 0.0f, 1, 1},
        { 0.5, -0.5, -0.5, 1, 0, 0, 0.0f,  -1.0f, 0.0f, 1, 0},
        {-0.5, -0.5, -0.5, 1, 0, 0, 0.0f,  -1.0f, 0.0f, 0, 0},

        {-0.5,  0.5, -0.5, 1, 0, 0, 0.0f, 1.0f, 0.0f, 0, 1},
        { 0.5,  0.5, -0.5, 1, 0, 0, 0.0f, 1.0f, 0.0f, 1, 1},
        { 0.5,  0.5,  0.5, 1, 0, 0, 0.0f, 1.0f, 0.0f, 1, 0},
        {-0.5,  0.5,  0.5, 1, 0, 0, 0.0f, 1.0f, 0.0f, 0, 0},

        { 0.5, -0.5, -0.5, 0, 0, 1, 1.0f,  0.0f,  0.0f, 0, 1},
        { 0.5, -0.5,  0.5, 0, 0, 1, 1.0f,  0.0f,  0.0f, 1, 1},
        { 0.5,  0.5,  0.5, 0, 0, 1, 1.0f,  0.0f,  0.0f, 1, 0},
        { 0.5,  0.5, -0.5, 0, 0, 1, 1.0f,  0.0f,  0.0f, 0, 0},

        {-0.5, -0.5,  0.5, 0, 0, -1, -1.0f,  0.0f,  0.0f, 0, 1},
        {-0.5, -0.5, -0.5, 0, 0, -1, -1.0f,  0.0f,  0.0f, 1, 1},
        {-0.5,  0.5, -0.5, 0, 0, -1, -1.0f,  0.0f,  0.0f, 1, 0},
        {-0.5,  0.5,  0.5, 0, 0, -1, -1.0f,  0.0f,  0.0f, 0, 0},

        { 0.5, -0.5,  0.5, -1, 0, 0, 0.0f, 0.0f, 1.0f, 0, 1},
        {-0.5, -0.5,  0.5, -1, 0, 0, 0.0f, 0.0f, 1.0f, 1, 1},
        {-0.5,  0.5,  0.5, -1, 0, 0, 0.0f, 0.0f, 1.0f, 1, 0},
        { 0.5,  0.5,  0.5, -1, 0, 0, 0.0f, 0.0f, 1.0f, 0, 0},

        {-0.5, -0.5, -0.5, 1, 0, 0, 0.0f, 0.0f, -1.0f, 0, 1},
        { 0.5, -0.5, -0.5, 1, 0, 0, 0.0f, 0.0f, -1.0f, 1, 1},
        { 0.5,  0.5, -0.5, 1, 0, 0, 0.0f, 0.0f, -1.0f, 1, 0},
        {-0.5,  0.5, -0.5, 1, 0, 0, 0.0f, 0.0f, -1.0f, 0, 0},
    };

    static const DWORD Indices[] = {
        0, 2, 1, 0, 3, 2,
        4, 6, 5, 4, 7, 6,
        8, 10, 9, 8, 11, 10,
        12, 14, 13, 12, 15, 14,
        16, 18, 17, 16, 19, 18,
        20, 22, 21, 20, 23, 22,
    };

    D3D11_BUFFER_DESC desc;
    ZeroMemory(&desc, sizeof(desc));

    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(v);
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(data));
    data.pSysMem = v;

    HRESULT hr = m_pDevice->CreateBuffer(&desc, &data, &m_pVertextBuffer);

    if (SUCCEEDED(hr))
    {
        desc = {};
        desc.ByteWidth = sizeof(DWORD) * ARRAYSIZE(Indices);
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        data.pSysMem = &Indices;
        data.SysMemPitch = sizeof(Indices);
        data.SysMemSlicePitch = 0;

        hr = m_pDevice->CreateBuffer(&desc, &data, &m_pIndexBuffer);
    }

    ID3D11Buffer* m_pSceneBuffer;

    if (SUCCEEDED(hr))
    {
        desc = { 0 };
        desc.ByteWidth = sizeof(SceneBuffer);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        hr = m_pDevice->CreateBuffer(&desc, NULL, &m_pSceneBuffer);
    }

    if (SUCCEEDED(hr))
    {
        constBuffers.push_back(m_pSceneBuffer);
    }

    ID3D11Buffer* m_pGeomBufferInst;

    if (SUCCEEDED(hr))
    {
        desc = { 0 };
        desc.ByteWidth = sizeof(GeomBuffer) * maxInstancesNum;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        hr = m_pDevice->CreateBuffer(&desc, NULL, &m_pGeomBufferInst);
    }

    if (SUCCEEDED(hr))
    {
        constBuffers.push_back(m_pGeomBufferInst);
    }

    ID3D11Buffer* m_pTextureNumInst;

    if (SUCCEEDED(hr))
    {
        desc = { 0 };
        desc.ByteWidth = sizeof(TextureNum) * maxInstancesNum;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        hr = m_pDevice->CreateBuffer(&desc, NULL, &m_pTextureNumInst);
    }

    if (SUCCEEDED(hr))
    {
        constBuffers.push_back(m_pTextureNumInst);
    }

    for (int i = 0; i < maxInstancesNum; i++)
    {
        translateMatrices.push_back(DirectX::XMMatrixTranslation(rand() % 10, rand() % 10, rand() % 10));
        scaleMatrices.push_back(DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f));
        rotateMatrices.push_back(DirectX::XMMatrixRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), 0.0f));
        geomBuffers.push_back(GeomBuffer());
        texNumBuffers.push_back({ DirectX::XMINT4(rand() % 2, 0, 0, 0) });
        rotateSpeed.push_back(rand() % 100 / 1000.0f);
        rotateAngle.push_back(0);
    }

    return hr;
}

HRESULT Cube::CreateShaders(ID3D11Device* m_pDevice)
{
    static const D3D11_INPUT_ELEMENT_DESC InputDesc[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    if (!vs.Initialize(m_pDevice, L"CubeVS.hlsl", nullptr))
    {
        return S_FALSE;
    }

    HRESULT result = m_pDevice->CreateInputLayout(InputDesc, ARRAYSIZE(InputDesc), vs.GetBuffer()->GetBufferPointer(), vs.GetBuffer()->GetBufferSize(), &m_pInputLayout);

    if (!ps.Initialize(m_pDevice, L"CubePS.hlsl", nullptr))
    {
        return S_FALSE;
    }

    return result;
}

HRESULT Cube::CreateTextures(ID3D11Device* m_pDevice)
{
    ID3D11SamplerState* m_pSampler;
    ID3D11ShaderResourceView* m_pTextureView;

    HRESULT result = DirectX::CreateDDSTextureFromFile(m_pDevice, L"textureWithNormalMap/breaks.dds", nullptr, &m_pTextureView);

    if (SUCCEEDED(result))
    {
        resources.push_back(m_pTextureView);

        D3D11_SAMPLER_DESC desc = {};
        desc.Filter = D3D11_FILTER_ANISOTROPIC;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.MinLOD = -FLT_MAX;
        desc.MaxLOD = FLT_MAX;
        desc.MipLODBias = 0.0f;
        desc.MaxAnisotropy = 16;
        desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 1.0f;
        result = m_pDevice->CreateSamplerState(&desc, &m_pSampler);
    }

    ID3D11ShaderResourceView* m_pTextureViewCat;

    result = DirectX::CreateDDSTextureFromFile(m_pDevice, L"cat.dds", nullptr, &m_pTextureViewCat);

    if (SUCCEEDED(result))
    {
        resources.push_back(m_pTextureViewCat);
    }

    ID3D11ShaderResourceView* m_pTextureView1;

    result = DirectX::CreateDDSTextureFromFile(m_pDevice, L"textureWithNormalMap/breaks_norm.dds", nullptr, &m_pTextureView1);

    if (SUCCEEDED(result))
    {
        resources.push_back(m_pTextureView1);
    }

    if (SUCCEEDED(result))
    {
        samplers.push_back(m_pSampler);
    }

    return result;
}

void Cube::Draw(const DirectX::XMMATRIX& vp,
	ID3D11DeviceContext* m_pDeviceContext)
{
    m_pDeviceContext->RSSetState(rasterizerState);

    for (int i = 0; i < numInstances; i++)
    {
        rotateAngle[i] += rotateSpeed[i];
        if (rotateAngle[i] > 6.28f)
            rotateAngle[i] = 0.0f;
        rotateMatrices[i] = DirectX::XMMatrixRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotateAngle[i]);
        geomBuffers[i].modelMatrix = scaleMatrices[i] * rotateMatrices[i] * translateMatrices[i];
        geomBuffers[i].normalTransform = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, geomBuffers[i].modelMatrix));
        geomBuffers[i].modelMatrix = DirectX::XMMatrixTranspose(geomBuffers[i].modelMatrix);
    }

    scBuffer.vp = vp;
    scBuffer.vp = DirectX::XMMatrixTranspose(scBuffer.vp);
    m_pDeviceContext->UpdateSubresource(constBuffers[0], 0, nullptr, &scBuffer, 0, 0);
    m_pDeviceContext->UpdateSubresource(constBuffers[1], 0, nullptr, geomBuffers.data(), 0, 0);
    m_pDeviceContext->UpdateSubresource(constBuffers[2], 0, nullptr, texNumBuffers.data(), 0, 0);

    m_pDeviceContext->IASetInputLayout(m_pInputLayout);

    m_pDeviceContext->VSSetShader(vs.GetShader(), NULL, 0);
    m_pDeviceContext->PSSetShader(ps.GetShader(), NULL, 0);

    m_pDeviceContext->VSSetConstantBuffers(0, 2, constBuffers.data());
    m_pDeviceContext->PSSetConstantBuffers(2, 1, &constBuffers[2]);

    if (!samplers.empty() && !resources.empty())
    {
        m_pDeviceContext->PSSetSamplers(0, 1, samplers.data());

        m_pDeviceContext->PSSetShaderResources(0, 3, resources.data());
    }

    m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertextBuffer, &stride, &offset);

    m_pDeviceContext->DrawIndexedInstanced(36, numInstances, 0, 0, 0);
}