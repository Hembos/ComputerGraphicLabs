#include "Shapes.h"
#include "DDSTextureLoader11.h"

#include <vector>
#include <math.h>

void Cube::Clean()
{
    vs.Clean();
    ps.Clean();
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(m_pVertextBuffer);
    for (auto& buf : constBuffers)
    {
        SAFE_RELEASE(buf);
    }
    for (auto& res : resources)
    {
        SAFE_RELEASE(res);
    }
    for (auto& sampler : samplers)
    {
        SAFE_RELEASE(sampler);
    }
    SAFE_RELEASE(m_pInputLayout);
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(rasterizerState);
}

HRESULT Cube::CreateGeometry(ID3D11Device* m_pDevice)
{
    static const Vertex v[] = {
        {-0.5, -0.5, 0.5, 0, 1},
        {0.5, -0.5, 0.5, 1, 1},
        {0.5, -0.5, -0.5, 1, 0},
        {-0.5, -0.5, -0.5, 0, 0},

        {-0.5,  0.5, -0.5, 0,1},
        { 0.5,  0.5, -0.5, 1,1},
        { 0.5,  0.5,  0.5, 1,0},
        {-0.5,  0.5,  0.5, 0,0},

        { 0.5, -0.5, -0.5, 0,1},
        { 0.5, -0.5,  0.5, 1,1},
        { 0.5,  0.5,  0.5, 1,0},
        { 0.5,  0.5, -0.5, 0,0},

        {-0.5, -0.5,  0.5, 0,1},
        {-0.5, -0.5, -0.5, 1,1},
        {-0.5,  0.5, -0.5, 1,0},
        {-0.5,  0.5,  0.5, 0,0},

        { 0.5, -0.5,  0.5, 0,1},
        {-0.5, -0.5,  0.5, 1,1},
        {-0.5,  0.5,  0.5, 1,0},
        { 0.5,  0.5,  0.5, 0,0},

        {-0.5, -0.5, -0.5, 0,1},
        { 0.5, -0.5, -0.5, 1,1},
        { 0.5,  0.5, -0.5, 1,0},
        {-0.5,  0.5, -0.5, 0,0},
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

    ID3D11Buffer* m_pGeomBuffer;

    if (SUCCEEDED(hr))
    {
        desc = { 0 };
        desc.ByteWidth = sizeof(GeomBuffer);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        hr = m_pDevice->CreateBuffer(&desc, NULL, &m_pGeomBuffer);
    }

    if (SUCCEEDED(hr))
    {
        constBuffers.push_back(m_pGeomBuffer);
    }

    return hr;
}

HRESULT Cube::CreateShaders(ID3D11Device* m_pDevice)
{
    static const D3D11_INPUT_ELEMENT_DESC InputDesc[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    if (!vs.Initialize(m_pDevice, L"VertexShader.cso"))
    {
        return S_FALSE;
    }

    HRESULT result = m_pDevice->CreateInputLayout(InputDesc, ARRAYSIZE(InputDesc), vs.GetBuffer()->GetBufferPointer(), vs.GetBuffer()->GetBufferSize(), &m_pInputLayout);

    if (!ps.Initialize(m_pDevice, L"PixelShader.cso"))
    {
        return S_FALSE;
    }

    return result;
}

HRESULT Cube::CreateTextures(ID3D11Device* m_pDevice)
{
    ID3D11SamplerState* m_pSampler;
    ID3D11ShaderResourceView* m_pTextureView;

    HRESULT result = DirectX::CreateDDSTextureFromFile(m_pDevice, L"cat.dds", nullptr, &m_pTextureView);

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

    if (SUCCEEDED(result))
    {
        samplers.push_back(m_pSampler);
    }

    return result;
}

void Cube::Draw(const DirectX::XMMATRIX& vp, ID3D11DeviceContext* m_pDeviceContext)
{
    m_pDeviceContext->RSSetState(rasterizerState);

    model = DirectX::XMMatrixIdentity();
    model = rotateMatrix * scaleMatrix * translateMatrix;
    geomBuffer.modelMatrix = model * vp;
    geomBuffer.modelMatrix = DirectX::XMMatrixTranspose(geomBuffer.modelMatrix);
    m_pDeviceContext->UpdateSubresource(constBuffers[0], 0, nullptr, &geomBuffer, 0, 0);

    m_pDeviceContext->IASetInputLayout(m_pInputLayout);

    m_pDeviceContext->VSSetShader(vs.GetShader(), NULL, 0);
    m_pDeviceContext->PSSetShader(ps.GetShader(), NULL, 0);

    m_pDeviceContext->VSSetConstantBuffers(0, 1, constBuffers.data());

    if (!samplers.empty() && !resources.empty())
    {
        m_pDeviceContext->PSSetSamplers(0, 1, samplers.data());

        m_pDeviceContext->PSSetShaderResources(0, 1, resources.data());
    }
    
    m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertextBuffer, &stride, &offset);

    m_pDeviceContext->DrawIndexed(36, 0, 0);
}

void Sphere::Clean()
{
    vs.Clean();
    ps.Clean();
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(m_pVertextBuffer);
    for (auto& buf : constBuffers)
    {
        SAFE_RELEASE(buf);
    }
    for (auto& res : resources)
    {
        SAFE_RELEASE(res);
    }
    for (auto& sampler : samplers)
    {
        SAFE_RELEASE(sampler);
    }
    SAFE_RELEASE(m_pInputLayout);
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(rasterizerState);
}

HRESULT Sphere::CreateGeometry(ID3D11Device* m_pDevice)
{
    static std::vector<Vertex> Vertices;

    static std::vector<DWORD> Indices;

    int stackCount = 20;
    int sliceCount = 20;
    float phiStep = DirectX::XM_PI / stackCount;
    float thetaStep = 2.0f * DirectX::XM_PI / sliceCount;
    float radius = 0.5f;
    Vertices.push_back({ 0, radius, 0 });

    for (int i = 1; i < stackCount; i++)
    {
        float phi = i * phiStep;
        for (int j = 0; j <= sliceCount; j++)
        {
            float theta = j * thetaStep;
            Vertex v = { radius * sin(phi) * cos(theta),
                                radius * cos(phi),
                                radius * sin(phi) * sin(theta) };
            Vertices.push_back(v);
        }
    }
    Vertices.push_back({ 0, -radius, 0 });

    for (int i = 1; i <= sliceCount; i++) {
        Indices.push_back(0);
        Indices.push_back(i + 1);
        Indices.push_back(i);
    }

    int baseIndex = 1;
    int ringVertexCount = sliceCount + 1;
    for (int i = 0; i < stackCount - 2; i++) {
        for (int j = 0; j < sliceCount; j++) {
            Indices.push_back(baseIndex + i * ringVertexCount + j);
            Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
            Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

            Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
            Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
            Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
        }
    }

    int southPoleIndex = Vertices.size() - 1;
    baseIndex = southPoleIndex - ringVertexCount;
    for (int i = 0; i < sliceCount; i++) {
        Indices.push_back(southPoleIndex);
        Indices.push_back(baseIndex + i);
        Indices.push_back(baseIndex + i + 1);
    }

    D3D11_SUBRESOURCE_DATA data;

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = Vertices.size() * sizeof(Vertex);
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;

    data.pSysMem = Vertices.data();
    data.SysMemPitch = Vertices.size() * sizeof(Vertex);
    data.SysMemSlicePitch = 0;

    HRESULT result = m_pDevice->CreateBuffer(&desc, &data, &m_pVertextBuffer);

    if (SUCCEEDED(result))
    {
        numIndeces = Indices.size();
        desc = {};
        desc.ByteWidth = Indices.size() * sizeof(DWORD);
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        data.pSysMem = Indices.data();
        data.SysMemPitch = Indices.size() * sizeof(DWORD);
        data.SysMemSlicePitch = 0;

        result = m_pDevice->CreateBuffer(&desc, &data, &m_pIndexBuffer);
    }

    ID3D11Buffer* m_pGeomBuffer;

    if (SUCCEEDED(result))
    {
        desc = { 0 };
        desc.ByteWidth = sizeof(GeomBuffer);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        result = m_pDevice->CreateBuffer(&desc, NULL, &m_pGeomBuffer);
    }

    if (SUCCEEDED(result))
    {
        constBuffers.push_back(m_pGeomBuffer);
    }

    return result;
}

HRESULT Sphere::CreateShaders(ID3D11Device* m_pDevice)
{
    static const D3D11_INPUT_ELEMENT_DESC InputDesc[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    if (!vs.Initialize(m_pDevice, L"SphereVS.cso"))
    {
        return S_FALSE;
    }

    HRESULT result = m_pDevice->CreateInputLayout(InputDesc, ARRAYSIZE(InputDesc), vs.GetBuffer()->GetBufferPointer(), vs.GetBuffer()->GetBufferSize(), &m_pInputLayout);

    if (!ps.Initialize(m_pDevice, L"SpherePS.cso"))
    {
        return S_FALSE;
    }

    return result;
}

HRESULT Sphere::CreateTextures(ID3D11Device* m_pDevice)
{
    ID3D11SamplerState* m_pSampler;
    ID3D11ShaderResourceView* m_pTextureView;

    const wchar_t* fileNames[] = { L"cubemap/posx.dds", L"cubemap/negx.dds", L"cubemap/posy.dds",
        L"cubemap/negy.dds", L"cubemap/posz.dds", L"cubemap/negz.dds" };
    HRESULT result = DirectX::CreateDDSCubeTextureFromFile(m_pDevice, fileNames, nullptr, &m_pTextureView);

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

    if (SUCCEEDED(result))
    {
        samplers.push_back(m_pSampler);
    }

    return result;
}

void Sphere::Draw(const DirectX::XMMATRIX& vp, ID3D11DeviceContext* m_pDeviceContext)
{
    m_pDeviceContext->RSSetState(rasterizerState);

    model = DirectX::XMMatrixIdentity();
    model = scaleMatrix;
    geomBuffer.modelMatrix = model * vp;
    geomBuffer.modelMatrix = DirectX::XMMatrixTranspose(geomBuffer.modelMatrix);
    geomBuffer.cameraPos = camPos;
    geomBuffer.radius = DirectX::XMVectorSet(radius, 0.0f, 0.0f, 0.0f);
    m_pDeviceContext->UpdateSubresource(constBuffers[0], 0, nullptr, &geomBuffer, 0, 0);

    m_pDeviceContext->IASetInputLayout(m_pInputLayout);

    m_pDeviceContext->VSSetShader(vs.GetShader(), NULL, 0);
    m_pDeviceContext->PSSetShader(ps.GetShader(), NULL, 0);

    m_pDeviceContext->VSSetConstantBuffers(0, 1, constBuffers.data());

    if (!samplers.empty() && !resources.empty())
    {
        m_pDeviceContext->PSSetSamplers(0, 1, samplers.data());

        m_pDeviceContext->PSSetShaderResources(0, 1, resources.data());
    }

    m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertextBuffer, &stride, &offset);

    m_pDeviceContext->DrawIndexed(numIndeces, 0, 0);
}

void Sphere::setCamPos(DirectX::XMVECTOR camPos)
{
    this->camPos = camPos;
}

void Sphere::setRadius(const float& fov, const float& nearPlane, const float& width, const float& height)
{
    float halfW = tanf(fov / 2) * nearPlane;
    float halfH = float(height / width) * halfW;
    radius = sqrtf(nearPlane * nearPlane + halfH * halfH + halfW * halfW) * 1.1f;
}

HRESULT Shape::setRasterizerState(ID3D11Device* m_pDevice, D3D11_CULL_MODE cullMode)
{
    HRESULT result;
    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

    rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = cullMode;
    result = m_pDevice->CreateRasterizerState(&rasterizerDesc, &rasterizerState);

    return result;
}

void Shape::Translate(DirectX::XMMATRIX translateMatrix)
{
    this->translateMatrix = translateMatrix;
}

void Shape::Scale(DirectX::XMMATRIX scaleMatrix)
{
    this->scaleMatrix = scaleMatrix;
}

void Shape::Rotate(DirectX::XMMATRIX rotateMatrix)
{
    this->rotateMatrix = rotateMatrix;
}
