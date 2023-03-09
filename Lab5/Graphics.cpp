#include "Graphics.h"
#include "DDSTextureLoader11.h"

#include <assert.h>
#include <string>

#define SAFE_RELEASE(p) if (p != NULL) { p->Release(); p = NULL; }

Graphics::~Graphics()
{
    cube.Clean();
    sphere.Clean();
    SAFE_RELEASE(m_pBackBufferRTV);
    SAFE_RELEASE(m_pSwapChain);
    SAFE_RELEASE(m_pDeviceContext);
    //SAFE_RELEASE(m_pDevice);

    ID3D11Debug* d3Debug = nullptr;
    m_pDevice->QueryInterface(IID_PPV_ARGS(&d3Debug));

    UINT references = m_pDevice->Release();
    if (references > 1)
    {
        d3Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
    }

    d3Debug->Release();
}

bool Graphics::InitDirectX(HWND hwnd, int width, int height)
{
    windowHeight = height;
    windowWidth = width;

    HRESULT result;

    IDXGIFactory* pFactory = NULL;
    result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
    assert(SUCCEEDED(result));

    IDXGIAdapter* pSelectedAdapter = NULL;

    if (SUCCEEDED(result))
    {
        IDXGIAdapter* pAdapter = NULL;
        UINT adapterIdx = 0;

        while (SUCCEEDED(pFactory->EnumAdapters(adapterIdx, &pAdapter)))
        {
            DXGI_ADAPTER_DESC desc;

            pAdapter->GetDesc(&desc);

            if (wcscmp(desc.Description, L"Microsoft Basic Render Driver") != 0)
            {
                pSelectedAdapter = pAdapter;
                break;
            }

            pAdapter->Release();
            adapterIdx++;
        }
    }
    assert(pSelectedAdapter != NULL);

    D3D_FEATURE_LEVEL level;
    D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0 };

    if (SUCCEEDED(result))
    {
        UINT flags = 0;
#ifdef _DEBUG
        flags = D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

        result = D3D11CreateDevice(pSelectedAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL,
            flags, levels, 1, D3D11_SDK_VERSION, &m_pDevice, &level, &m_pDeviceContext);
    }

    if (SUCCEEDED(result))
    {
        DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
        swapChainDesc.BufferCount = 2;
        swapChainDesc.BufferDesc.Width = width;
        swapChainDesc.BufferDesc.Height = height;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = hwnd;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.Windowed = true;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.Flags = 0;

        result = pFactory->CreateSwapChain(m_pDevice, &swapChainDesc, &m_pSwapChain);
    }

    if (SUCCEEDED(result))
    {
        ID3D11Texture2D* pBackBuffer = NULL;
        result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
        if (SUCCEEDED(result))
        {
            result = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pBackBufferRTV);

            SAFE_RELEASE(pBackBuffer);
        }
    }

    ID3D11RenderTargetView* views[] = { m_pBackBufferRTV };
    m_pDeviceContext->OMSetRenderTargets(1, views, nullptr);

    D3D11_VIEWPORT viewport;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (FLOAT)width;
    viewport.Height = (FLOAT)height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    m_pDeviceContext->RSSetViewports(1, &viewport);

    if (SUCCEEDED(result))
    {
        result = cube.setRasterizerState(m_pDevice, D3D11_CULL_MODE::D3D11_CULL_BACK);
        result = sphere.setRasterizerState(m_pDevice, D3D11_CULL_MODE::D3D11_CULL_FRONT);
    }

    cube.CreateTextures(m_pDevice);
    sphere.CreateTextures(m_pDevice);
    
    SAFE_RELEASE(pSelectedAdapter);
    SAFE_RELEASE(pFactory);

    return SUCCEEDED(result);
}

HRESULT SetResourceName(ID3D11DeviceChild* pResource, const std::string name)
{
    return pResource->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)name.length(), name.c_str());
}

bool Graphics::InitShaders()
{
    HRESULT result = cube.CreateShaders(m_pDevice);

    if (SUCCEEDED(result))
    {
        result = sphere.CreateShaders(m_pDevice);
    }

    return SUCCEEDED(result);
}

bool Graphics::InitScene()
{
    HRESULT hr = cube.CreateGeometry(m_pDevice);

    if (SUCCEEDED(hr))
    {
        hr = sphere.CreateGeometry(m_pDevice);
    }

    camera.SetPosition(DirectX::XMVectorSet(-2.0f, 0.0f, 0.0f, 0.0));
    camera.SetProjectionValues(100.0f, (float)windowHeight / windowWidth, 0.1f, 1000.0f);
    camera.AdjustRotation(DirectX::XMVectorSet(0.0f, DirectX::XM_PIDIV2, 0.0f, 1.0f));

    sphere.setRadius(camera.GetFov(), camera.GetNearPlane(), windowWidth, windowHeight);

    return SUCCEEDED(hr);
}

void Graphics::RenderFrame()
{
    ID3D11RenderTargetView* views[] = { m_pBackBufferRTV };
    m_pDeviceContext->OMSetRenderTargets(1, views, nullptr);

    static const FLOAT BackColor[4] = { 0.25f, 0.25f, 0.25f, 1.0f };
    m_pDeviceContext->ClearRenderTargetView(m_pBackBufferRTV, BackColor);

    D3D11_VIEWPORT viewport;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (FLOAT)windowWidth;
    viewport.Height = (FLOAT)windowHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    m_pDeviceContext->RSSetViewports(1, &viewport);

    D3D11_RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = windowWidth;
    rect.bottom = windowHeight;
    m_pDeviceContext->RSSetScissorRects(1, &rect);

    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    DirectX::XMMATRIX vp = camera.GetViewMatrix()* camera.GetProjectionMatrix();
    cube.Translate(DirectX::XMMatrixTranslation(1.0f, 0.0f, 0.0f));

    sphere.setCamPos(camera.GetPositionVector());

    sphere.Draw(vp, m_pDeviceContext);
    cube.Draw(vp, m_pDeviceContext);

    HRESULT result = m_pSwapChain->Present(0, 0);
    assert(SUCCEEDED(result));
}

Camera& Graphics::GetCamera()
{
    return camera;
}

void Graphics::Resize(const int& width, const int& height)
{
    if ((width != windowWidth || height != windowHeight) && m_pSwapChain != nullptr)
    {
        if (m_pBackBufferRTV != NULL)
        {
            m_pBackBufferRTV->Release();
            m_pBackBufferRTV = NULL;
        }

        HRESULT result = m_pSwapChain->ResizeBuffers(2, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
        if (SUCCEEDED(result))
        {
            windowWidth = width;
            windowHeight = height;

            ID3D11Texture2D* pBackBuffer = NULL;
            HRESULT result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
            if (SUCCEEDED(result))
            {
                result = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pBackBufferRTV);

                if (pBackBuffer != NULL)
                {
                    pBackBuffer->Release();
                    pBackBuffer = NULL;
                }
            }

            sphere.setRadius(camera.GetFov(), camera.GetNearPlane(), windowWidth, windowHeight);

            assert(SUCCEEDED(result));
        }
    }
}
