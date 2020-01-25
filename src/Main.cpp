#include "stdafx.h"
#include "VansEngine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	VansEngine van(1280, 720, L"VansEngine");
    return Win32Application::Run(&van, hInstance, nCmdShow);
}
