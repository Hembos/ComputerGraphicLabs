#pragma once

#include "Shapes/Sphere.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

class Light
{
private:
	struct SceneBuffer
	{
		DirectX::XMMATRIX vp;
		DirectX::XMVECTOR cameraPos;
	};

	struct PointLight
	{
		float pos[4];
		float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	};

	struct LightBuffer
	{
		PointLight lightsDesc[10];
		float ambientColor[4] = { 0.5f, 0.5f, 0.5f, 0.5f }; // (r,g,b,a): a - intensity, rgb - color
		int lightsCount = 0;
		float diffuseCoef = 0.5f;
		float specularCoef = 0.9f;
		float shine = 20.0f;
	};
public:
	HRESULT Initialize(ID3D11Device* m_pDevice);
	void AddLight();
	void Draw(const DirectX::XMMATRIX& vp,
		ID3D11DeviceContext* m_pDeviceContext);
	void RenderImGUI();

	void UpdateBuffer(ID3D11DeviceContext* m_pDeviceContext);

	void SetCamPos(DirectX::XMVECTOR camPos);

	void Clean();
private:
	Sphere lightInstances;

	LightBuffer light;
	ID3D11Buffer* lightBuffer;

	SceneBuffer scBuffer;
	ID3D11Buffer* sceneBuffer;
};

