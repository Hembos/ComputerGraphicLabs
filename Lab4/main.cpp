#include <Windows.h>

#include "Engine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    Engine engine(hInstance, 800, 600, L"WindowClass", L"Engine");

    if (!engine.Init(nCmdShow))
    {
        return -1;
    }

    while (engine.ProcessMessages())
    {
        engine.Update();
    }    

    return 0;
}
