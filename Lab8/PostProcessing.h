#pragma once

#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

#include "Shaders.h"

#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment( lib, "dxguid.lib")

class PostProcessing
{
	struct PostProcBuffer
	{
		DirectX::XMFLOAT4 alpha = { DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f) };
		DirectX::XMINT4 usesOper = { DirectX::XMINT4(0, 0, 0, 0) };
	};
public:
	HRESULT CreateShaders(ID3D11Device* m_pDevice);
	HRESULT CreateBuffers(ID3D11Device* m_pDevice, int m_height, int m_width);
	void Draw(ID3D11DeviceContext* m_pDeviceContext, ID3D11RenderTargetView* m_pBackBufferRTV);
	HRESULT Resize(ID3D11Device* m_pDevice, int m_height, int m_width);
	void SetTargetInColorRTV(ID3D11DeviceContext* m_pDeviceContext, ID3D11DepthStencilView* m_pDepthBufferDSV);
	void RenderImGUI();
	void Clean();
private:
	VertexShader vs;
	PixelShader ps;
    ID3D11Texture2D* m_pColorBuffer = NULL;
    ID3D11ShaderResourceView* m_pColorBufferSRV = NULL;
	ID3D11RenderTargetView* m_pColorBufferRTV = NULL;
	ID3D11SamplerState* sampler;

	PostProcBuffer postProc;
	ID3D11Buffer* postProcBuffer;
};

