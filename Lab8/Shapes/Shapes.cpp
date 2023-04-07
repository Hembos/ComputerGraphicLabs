#include "Shapes.h"

HRESULT InstanceShape::setRasterizerState(ID3D11Device* m_pDevice, D3D11_CULL_MODE cullMode)
{
    HRESULT result;
    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

    rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = cullMode;
    result = m_pDevice->CreateRasterizerState(&rasterizerDesc, &rasterizerState);

    return result;
}

void InstanceShape::addInstance()
{
    if (numInstances < maxInstancesNum)
    {
        numInstances++;
    }
}

void InstanceShape::Translate(DirectX::XMMATRIX translateMatrix, int ind)
{
    translateMatrices[ind] = translateMatrix;
}

void InstanceShape::Scale(DirectX::XMMATRIX scaleMatrix, int ind)
{
    scaleMatrices[ind] = scaleMatrix;
}

void InstanceShape::Rotate(DirectX::XMMATRIX rotateMatrix, int ind)
{
    rotateMatrices[ind] = rotateMatrix;
}

int InstanceShape::getNumInstances()
{
    return numInstances;
}

void InstanceShape::Clean()
{
    vs.Clean();
    ps.Clean();
    cs.Clean();
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(m_pVertextBuffer);
    SAFE_RELEASE(m_pIndirectArgsSrc);
    SAFE_RELEASE(m_pIndirectArgsUAV);
    SAFE_RELEASE(m_pIndirectArgs);
    SAFE_RELEASE(m_pGeomBufferInstVisGPU);
    SAFE_RELEASE(m_pGeomBufferInstVis);
    SAFE_RELEASE(m_pGeomBufferInstVisGPU_UAV);
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
