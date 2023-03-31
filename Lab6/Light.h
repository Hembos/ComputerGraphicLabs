#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

#include "Shapes.h"

#pragma comment(lib, "d3d11")

class Light
{
	struct PointLight
	{
		float pos[3];
		Sphere sphere;
	};

	struct LightBuffer
	{
		float ambientColor[4]; // (r,g,b,a): a - intensity, rgb - color
	};
public:
	Light() = default;
	~Light() = default;
	HRESULT createBuffer(ID3D11Device* device);
	void updateBuffer(ID3D11DeviceContext* m_pDeviceContext);
	void setAmbient(float r, float g, float b, float intensity);
	void RenderImGUI(ID3D11Device* device, ID3D11DeviceContext* m_pDeviceContext);
	void addLight(ID3D11Device* device, ID3D11DeviceContext* m_pDeviceContext);

	void Draw(const DirectX::XMMATRIX& vp,
		ID3D11DeviceContext* m_pDeviceContext);

	void Clean();
private:
	LightBuffer light;
	ID3D11Buffer* lightBuffer;
	
	std::vector<PointLight> lightsShapes;
};

