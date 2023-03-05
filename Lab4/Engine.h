#pragma once

#include "RenderWindow.h"
#include "Graphics.h"

#include <memory>

class Engine
{
public:
	Engine(const HINSTANCE& hInstance, const int& width, const int& height, const std::wstring& windowClass, const std::wstring& title);
	bool Init(int nCmdShow);
	bool ProcessMessages();
	void Update();
private:
	std::unique_ptr<RenderWindow> window;
	std::unique_ptr<Graphics> graphics;
};

