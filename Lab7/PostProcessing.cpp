#include "PostProcessing.h"

HRESULT PostProcessing::CreateShaders(ID3D11Device* m_pDevice)
{
    if (!vs.Initialize(m_pDevice, L"PostProcVS.hlsl", nullptr))
    {
        return S_FALSE;
    }

    if (!ps.Initialize(m_pDevice, L"PostProcPS.hlsl", nullptr))
    {
        return S_FALSE;
    }

    return S_OK;
}

HRESULT PostProcessing::CreateBuffers(ID3D11Device* m_pDevice, int m_height, int m_width)
{
    D3D11_TEXTURE2D_DESC desc;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.ArraySize = 1;
    desc.MipLevels = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Height = m_height;
    desc.Width = m_width;
    HRESULT result = m_pDevice->CreateTexture2D(&desc, nullptr, &m_pColorBuffer);

    if (SUCCEEDED(result))
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        desc.Texture2D.MipLevels = 1;
        desc.Texture2D.MostDetailedMip = 0;
        result = m_pDevice->CreateShaderResourceView(m_pColorBuffer, &desc, &m_pColorBufferSRV);
    }

    if (SUCCEEDED(result))
    {
        result = m_pDevice->CreateRenderTargetView(m_pColorBuffer, nullptr, &m_pColorBufferRTV);
    }

    if (SUCCEEDED(result))
    {
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
        result = m_pDevice->CreateSamplerState(&desc, &sampler);
    }

    if (SUCCEEDED(result))
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.ByteWidth = sizeof(PostProcBuffer);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        result = m_pDevice->CreateBuffer(&desc, NULL, &postProcBuffer);
    }

    return result;
}

void PostProcessing::Draw(ID3D11DeviceContext* m_pDeviceContext, ID3D11RenderTargetView* m_pBackBufferRTV)
{
    ID3D11RenderTargetView* views[] = { m_pBackBufferRTV };
    m_pDeviceContext->OMSetRenderTargets(1, views, nullptr);
    ID3D11SamplerState* samplers[] = { sampler };
    m_pDeviceContext->PSSetSamplers(0, 1, samplers);

    ID3D11ShaderResourceView* resources[] = { m_pColorBufferSRV };
    m_pDeviceContext->PSSetShaderResources(0, 1, resources);
    m_pDeviceContext->OMSetDepthStencilState(nullptr, 0);
    m_pDeviceContext->RSSetState(nullptr);
    m_pDeviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
    m_pDeviceContext->IASetInputLayout(nullptr);
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pDeviceContext->UpdateSubresource(postProcBuffer, 0, nullptr, &postProc, 0, 0);
    m_pDeviceContext->PSSetConstantBuffers(0, 1, &postProcBuffer);
    m_pDeviceContext->VSSetShader(vs.GetShader(), nullptr, 0);
    m_pDeviceContext->PSSetShader(ps.GetShader(), nullptr, 0);
    m_pDeviceContext->Draw(3, 0);
}

HRESULT PostProcessing::Resize(ID3D11Device* m_pDevice, int m_height, int m_width)
{
    SAFE_RELEASE(m_pColorBuffer);
    SAFE_RELEASE(m_pColorBufferSRV);

    CreateBuffers(m_pDevice, m_height, m_width);

    return E_NOTIMPL;
}

void PostProcessing::SetTargetInColorRTV(ID3D11DeviceContext* m_pDeviceContext, ID3D11DepthStencilView* m_pDepthBufferDSV)
{
    ID3D11RenderTargetView* views[] = { m_pColorBufferRTV };
    m_pDeviceContext->OMSetRenderTargets(1, views, m_pDepthBufferDSV);

    static const FLOAT BackColor[4] = { 0.25f, 0.25f, 0.25f, 1.0f };
    m_pDeviceContext->ClearRenderTargetView(m_pColorBufferRTV, BackColor);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthBufferDSV, D3D11_CLEAR_DEPTH, 0.0f, 0);
}

void PostProcessing::RenderImGUI()
{
    static bool useSepia = false;
    static bool useContrast = false;

    ImGui::Begin("Post processing");

    ImGui::Checkbox("Sepia", &useSepia);
    postProc.usesOper.x = useSepia ? 1 : 0;

    ImGui::Checkbox("Contrast", &useContrast);
    postProc.usesOper.y = useContrast ? 1 : 0;
    if (useContrast)
    {
        ImGui::SliderFloat("Contrast alpha", &postProc.alpha.x, 0.0f, 10.0f);
    }

    ImGui::End();
}

void PostProcessing::Clean()
{
    vs.Clean();
    ps.Clean();
    SAFE_RELEASE(m_pColorBuffer);
    SAFE_RELEASE(m_pColorBufferSRV);
    SAFE_RELEASE(m_pColorBufferRTV);
    SAFE_RELEASE(postProcBuffer);
    SAFE_RELEASE(sampler);
}
