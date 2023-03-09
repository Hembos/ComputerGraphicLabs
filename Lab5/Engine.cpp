#include "Engine.h"
#include "Keyboard.h"
#include "Mouse.h"

Engine::Engine(const HINSTANCE& hInstance, const int& width, const int& height, const std::wstring& windowClass, const std::wstring& title)
{
    window = std::make_unique<RenderWindow>(hInstance, width, height, windowClass, title);
    graphics = std::make_unique<Graphics>();
}

bool Engine::Init(int nCmdShow)
{
    if (!window->Init(nCmdShow))
    {
        return false;
    }

    if (!graphics->InitDirectX(window->getHwnd(), window->getWidth(), window->getHeight()))
    {
        return false;
    }

    if (!graphics->InitShaders())
    {
        return false;
    }

    if (!graphics->InitScene())
    {
        return false;
    }

    return true;
}

bool Engine::ProcessMessages()
{
    return window->ProcessMessages();
}

void Engine::Update()
{
    while (!Keyboard::getInstance().KeyBufferIsEmpty())
    {
        KeyboardEvent kbe = Keyboard::getInstance().ReadKey();
        unsigned char keycode = kbe.GetKeyCode();
    }

    while (!Mouse::getInstance().EventBufferIsEmpty())
	{
		MouseEvent me = Mouse::getInstance().ReadEvent();

        if (Mouse::getInstance().IsRightDown())
        {
            if (me.GetType() == MouseEvent::EventType::Move)
            {
                graphics->GetCamera().AdjustRotation(DirectX::XMVectorSet((float)Mouse::getInstance().GetPosChange().y * 0.01f, (float)Mouse::getInstance().GetPosChange().x * 0.01f, 0, 1.0f));
            }
        }
	}

	const float cameraSpeed = 0.001f;

	if (Keyboard::getInstance().KeyIsPressed('W'))
	{
		graphics->GetCamera().AdjustPosition(DirectX::XMVectorScale(graphics->GetCamera().GetForwardVector(), cameraSpeed));
	}
	if (Keyboard::getInstance().KeyIsPressed('S'))
	{
		graphics->GetCamera().AdjustPosition(DirectX::XMVectorScale(graphics->GetCamera().GetBackwardVector(), cameraSpeed));
	}
	if (Keyboard::getInstance().KeyIsPressed('A'))
	{
		graphics->GetCamera().AdjustPosition(DirectX::XMVectorScale(graphics->GetCamera().GetLeftVector(), cameraSpeed));
	}
	if (Keyboard::getInstance().KeyIsPressed('D'))
	{
		graphics->GetCamera().AdjustPosition(DirectX::XMVectorScale(graphics->GetCamera().GetRightVector(), cameraSpeed));
	}

    graphics->RenderFrame();
}
