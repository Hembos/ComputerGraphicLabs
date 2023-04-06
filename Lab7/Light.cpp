#include "Light.h"

#include <string>

HRESULT Light::Initialize(ID3D11Device* m_pDevice)
{
    HRESULT result = lightInstances.CreateShaders(m_pDevice);

    if (SUCCEEDED(result))
    {
        result = lightInstances.CreateGeometry(m_pDevice);
    }

    if (SUCCEEDED(result))
    {
        result = lightInstances.setRasterizerState(m_pDevice, D3D11_CULL_MODE::D3D11_CULL_BACK);
    }

    D3D11_BUFFER_DESC desc;
    desc = { 0 };
    desc.ByteWidth = sizeof(LightBuffer);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;

    result = m_pDevice->CreateBuffer(&desc, NULL, &lightBuffer);

    if (SUCCEEDED(result))
    {
        desc = { 0 };
        desc.ByteWidth = sizeof(SceneBuffer);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        result = m_pDevice->CreateBuffer(&desc, NULL, &sceneBuffer);
    }

    return result;
}

void Light::AddLight()
{
    lightInstances.addInstance();
    lightInstances.Scale(DirectX::XMMatrixScaling(0.2f, 0.2f, 0.2f), lightInstances.getNumInstances() - 1);
    light.lightsCount++;
}

void Light::Draw(const DirectX::XMMATRIX& vp,
    ID3D11DeviceContext* m_pDeviceContext)
{
    for (int i = 0; i < lightInstances.getNumInstances(); i++)
    {
        lightInstances.setColor(DirectX::XMVectorSet(light.lightsDesc[i].color[0], light.lightsDesc[i].color[1], light.lightsDesc[i].color[2], 1.0f), i);
        lightInstances.Translate(DirectX::XMMatrixTranslation(light.lightsDesc[i].pos[0], light.lightsDesc[i].pos[1], light.lightsDesc[i].pos[2]), i);
    }

    lightInstances.Draw(vp, m_pDeviceContext);
}

void Light::RenderImGUI()
{
    ImGui::Begin("Light");

    ImGui::DragFloat4("Ambient", light.ambientColor, 0.01f);
    ImGui::DragFloat("Diffuse coefficient", &light.diffuseCoef, 0.01f);
    ImGui::DragFloat("Specular coefficient", &light.specularCoef, 0.01f);
    ImGui::DragFloat("Shine coefficient", &light.shine, 0.01f);
    if (ImGui::Button("+"))
    {
        AddLight();
    }
    for (int i = 0; i < lightInstances.getNumInstances(); i++)
    {
        std::string title = "Light " + std::to_string(i);
        ImGui::Text(title.c_str());
        std::string posTitle = "Pos" + std::to_string(i);
        ImGui::DragFloat3(posTitle.c_str(), light.lightsDesc[i].pos, 0.01f);
        std::string colorTitle = "Color" + std::to_string(i);
        ImGui::DragFloat3(colorTitle.c_str(), light.lightsDesc[i].color, 0.01f);
    }

    ImGui::End();
}

void Light::UpdateBuffer(ID3D11DeviceContext* m_pDeviceContext)
{
    m_pDeviceContext->UpdateSubresource(lightBuffer, 0, nullptr, &light, 0, 0);
    m_pDeviceContext->UpdateSubresource(sceneBuffer, 0, nullptr, &scBuffer, 0, 0);
    m_pDeviceContext->PSSetConstantBuffers(1, 1, &lightBuffer);
    m_pDeviceContext->PSSetConstantBuffers(0, 1, &sceneBuffer);
}

void Light::SetCamPos(DirectX::XMVECTOR camPos)
{
    scBuffer.cameraPos = camPos;
}

void Light::Clean()
{
    SAFE_RELEASE(lightBuffer);
    SAFE_RELEASE(sceneBuffer);
    lightInstances.Clean();
}