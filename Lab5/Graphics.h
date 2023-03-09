#pragma once

#include "Camera.h"
#include "Shapes.h"

#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>

#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment( lib, "dxguid.lib")

class Graphics
{
public:
	~Graphics();

	bool InitDirectX(HWND hwnd, int width, int height);
	bool InitShaders();
	bool InitScene();
	void RenderFrame();

	Camera& GetCamera();
private:
	int windowWidth;
	int windowHeight;

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pBackBufferRTV = nullptr;

	Camera camera;
	Cube cube;
	Sphere sphere;
};

