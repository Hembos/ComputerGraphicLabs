#include "Sphere.h"
#include "../DDSTextureLoader11.h"

#include <DirectXCollision.h>

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

    ID3D11Buffer* m_pSceneBuffer;

    if (SUCCEEDED(result))
    {
        desc = { 0 };
        desc.ByteWidth = sizeof(SceneBuffer);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        result = m_pDevice->CreateBuffer(&desc, NULL, &m_pSceneBuffer);
    }

    if (SUCCEEDED(result))
    {
        constBuffers.push_back(m_pSceneBuffer);
    }

    maxInstancesNum = 10;

    ID3D11Buffer* m_pGeomBuffer;

    if (SUCCEEDED(result))
    {
        desc = { 0 };
        desc.ByteWidth = sizeof(GeomBuffer) * maxInstancesNum;
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

    ID3D11Buffer* m_pColorBuffer;

    if (SUCCEEDED(result))
    {
        desc = { 0 };
        desc.ByteWidth = sizeof(ColorBuffer) * maxInstancesNum;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        result = m_pDevice->CreateBuffer(&desc, NULL, &m_pColorBuffer);
    }

    if (SUCCEEDED(result))
    {
        constBuffers.push_back(m_pColorBuffer);
    }

    ID3D11Buffer* m_pVisibleInstInd;

    if (SUCCEEDED(result))
    {
        desc = { 0 };
        desc.ByteWidth = sizeof(VisibleIndexes) * maxInstancesNum;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        result = m_pDevice->CreateBuffer(&desc, NULL, &m_pVisibleInstInd);
    }

    if (SUCCEEDED(result))
    {
        constBuffers.push_back(m_pVisibleInstInd);
    }

    for (int i = 0; i < maxInstancesNum; i++)
    {
        translateMatrices.push_back(DirectX::XMMatrixTranslation(rand() % 10, rand() % 10, rand() % 10));
        scaleMatrices.push_back(DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f));
        rotateMatrices.push_back(DirectX::XMMatrixRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), 0.0f));
        geomBuffers.push_back(GeomBuffer());
        colorBuffers.push_back(ColorBuffer());
        visibleIndBuffer.push_back({ DirectX::XMINT4(0, 0, 0, 0) });
    }

    return result;
}

HRESULT Sphere::CreateShaders(ID3D11Device* m_pDevice)
{
    static const D3D11_INPUT_ELEMENT_DESC InputDesc[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    if (!vs.Initialize(m_pDevice, L"LightSphereVS.hlsl", nullptr))
    {
        return S_FALSE;
    }

    HRESULT result = m_pDevice->CreateInputLayout(InputDesc, ARRAYSIZE(InputDesc), vs.GetBuffer()->GetBufferPointer(), vs.GetBuffer()->GetBufferSize(), &m_pInputLayout);

    if (!ps.Initialize(m_pDevice, L"LightSpherePS.hlsl", nullptr))
    {
        return S_FALSE;
    }

    return result;
}

HRESULT Sphere::CreateTextures(ID3D11Device* m_pDevice)
{
    return S_OK;
}

void Sphere::Draw(const DirectX::XMMATRIX& projMatrix, const DirectX::XMMATRIX& viewMatrix,
    ID3D11DeviceContext* m_pDeviceContext)
{
    m_pDeviceContext->RSSetState(rasterizerState);

    DirectX::BoundingFrustum fr(projMatrix, true);
    DirectX::XMMATRIX inverseViewMatrix = DirectX::XMMatrixInverse(nullptr, viewMatrix);
    fr.Transform(fr, inverseViewMatrix);

    visibleObjectNum = 0;

    for (int i = 0; i < numInstances; i++)
    {
        geomBuffers[i].modelMatrix = scaleMatrices[i] * rotateMatrices[i] * translateMatrices[i];
        geomBuffers[i].modelMatrix = DirectX::XMMatrixTranspose(geomBuffers[i].modelMatrix);

        DirectX::BoundingBox box(DirectX::XMFLOAT3(translateMatrices[i].r[3].m128_f32[0],
            translateMatrices[i].r[3].m128_f32[1],
            translateMatrices[i].r[3].m128_f32[2]),
            DirectX::XMFLOAT3(0.1, 0.1, 0.1));
        if (fr.Contains(box))
        {
            visibleIndBuffer[visibleObjectNum++].idx.x = i;
        }
    }

    scBuffer.vp = viewMatrix * projMatrix;
    scBuffer.vp = DirectX::XMMatrixTranspose(scBuffer.vp);
    m_pDeviceContext->UpdateSubresource(constBuffers[0], 0, nullptr, &scBuffer, 0, 0);
    m_pDeviceContext->UpdateSubresource(constBuffers[1], 0, nullptr, geomBuffers.data(), 0, 0);
    m_pDeviceContext->UpdateSubresource(constBuffers[2], 0, nullptr, colorBuffers.data(), 0, 0);
    m_pDeviceContext->UpdateSubresource(constBuffers[3], 0, nullptr, visibleIndBuffer.data(), 0, 0);

    m_pDeviceContext->IASetInputLayout(m_pInputLayout);

    m_pDeviceContext->VSSetShader(vs.GetShader(), NULL, 0);
    m_pDeviceContext->PSSetShader(ps.GetShader(), NULL, 0);

    m_pDeviceContext->VSSetConstantBuffers(0, 2, constBuffers.data());
    m_pDeviceContext->VSSetConstantBuffers(2, 1, &constBuffers[3]);
    m_pDeviceContext->PSSetConstantBuffers(0, 1, &constBuffers[2]);

    if (!samplers.empty() && !resources.empty())
    {
        m_pDeviceContext->PSSetSamplers(0, 1, samplers.data());

        m_pDeviceContext->PSSetShaderResources(0, 2, resources.data());
    }

    m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertextBuffer, &stride, &offset);

    m_pDeviceContext->DrawIndexedInstanced(numIndeces, visibleObjectNum, 0, 0, 0);
}

void Sphere::setColor(const DirectX::XMVECTOR& color, int idx)
{
    colorBuffers[idx].color = color;
}
