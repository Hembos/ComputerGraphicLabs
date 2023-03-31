#include "Light.h"

#include <string>

#define SAFE_RELEASE(p) if (p != NULL) { p->Release(); p = NULL; }

HRESULT Light::createBuffer(ID3D11Device* device)
{
    D3D11_BUFFER_DESC desc;
    desc = { 0 };
    desc.ByteWidth = sizeof(LightBuffer);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;

    HRESULT hr = device->CreateBuffer(&desc, NULL, &lightBuffer);
    
    return hr;
}

void Light::updateBuffer(ID3D11DeviceContext* m_pDeviceContext)
{
    m_pDeviceContext->UpdateSubresource(lightBuffer, 0, nullptr, &light, 0, 0);
    m_pDeviceContext->PSSetConstantBuffers(0, 1, &lightBuffer);
}

void Light::setAmbient(float r, float g, float b, float intensity)
{
    light.ambientColor[0] = r;
    light.ambientColor[1] = g;
    light.ambientColor[2] = b;
    light.ambientColor[3] = intensity;
}

void Light::RenderImGUI(ID3D11Device* device, ID3D11DeviceContext* m_pDeviceContext)
{
    ImGui_ImplWin32_NewFrame();
    ImGui_ImplDX11_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Light");

    ImGui::DragFloat4("Ambient", light.ambientColor, 0.01f);

    if (ImGui::Button("+") && lightsShapes.size() < 10)
    {
        addLight(device, m_pDeviceContext);
    }

    for (int i = 0; i < lightsShapes.size(); i++)
    {
        std::string title = "LightPos" + std::to_string(i);
        ImGui::DragFloat3(title.c_str(), lightsShapes[i].pos, 0.01f);
    }

    ImGui::End();

    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Light::addLight(ID3D11Device* device, ID3D11DeviceContext* m_pDeviceContext)
{
    Sphere sphere;

    sphere.setRasterizerState(device, D3D11_CULL_MODE::D3D11_CULL_BACK);
    sphere.CreateShaders(device);
    sphere.CreateGeometry(device);
    sphere.setColor(DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
    sphere.Scale(DirectX::XMMatrixScaling(0.2f, 0.2f, 0.2f));

    PointLight pointLight;
    pointLight.sphere = sphere;
    pointLight.pos[0] = 2.0f;
    pointLight.pos[1] = 2.0f;
    pointLight.pos[2] = 0.0f;

    lightsShapes.push_back(pointLight);
}

void Light::Draw(const DirectX::XMMATRIX& vp, ID3D11DeviceContext* m_pDeviceContext)
{
    for (auto& lightShape : lightsShapes)
    {
        lightShape.sphere.Translate(DirectX::XMMatrixTranslation(lightShape.pos[0], lightShape.pos[1], lightShape.pos[2]));
        lightShape.sphere.Draw(vp, m_pDeviceContext);
    }
}

void Light::Clean()
{
    SAFE_RELEASE(lightBuffer);
    for (auto& lightShape : lightsShapes)
    {
        lightShape.sphere.Clean();
    }
}
