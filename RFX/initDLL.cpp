#include <Windows.h>
#include "bs\window.h"
#include "BF2.h"
#include "IAT.h"
#include "HookedDirect3D9.h"

// Enable assertion even thought we are actually compiling a release build.
#undef NDEBUG
#define __DEBUG
#include <assert.h>
#undef __DEBUG
#define NDEBUG

// DUMMY FUNCTION WHICH WILL BE IMPORTED BY BF2.exe(RFX.exe)
__declspec(dllexport) int main();
int main()
{
	return 0;
}

// resolution of primary display
// TODO: support for multiple monitors?
bs::TUInt2 get_resolution()
{
	DEVMODE devMode;
	ZeroMemory(&devMode, sizeof(devMode));
	devMode.dmSize = sizeof(devMode);
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);
	return bs::TUInt2(devMode.dmPelsWidth, devMode.dmPelsHeight);
}


bs::IWindow * pWindow = nullptr;

auto pOrigCreateWindowExA = CreateWindowExA;
HWND WINAPI MyCreateWindowExA(
	DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName,
	DWORD dwStyle, int X, int Y, int nWidth, int nHeight,
	HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	HWND retVal = pOrigCreateWindowExA(dwExStyle, lpClassName, "RFX"/*lpWindowName*/,
		WS_VISIBLE, 0, 0, get_resolution().x(), get_resolution().y(), hWndParent, hMenu, hInstance, nullptr/*lpParam*/);

	pWindow = new bs::IWindow(retVal);
	BF2::init(pWindow);
	BF2::createConsoleObject(bs::lambda([](std::string val)
	{
		MessageBoxA(0, val.c_str(), "", MB_OK);
	}), "GameMessageBox");

	return retVal;
}

auto pOrigPeekMessage = PeekMessageA;
BOOL WINAPI MyPeekMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
	auto retVal = pOrigPeekMessage(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);

	if (!retVal) // there are no messages
	{
		// This is usefull for testing stuff and also serves as a sort of mainloop for .con "scripts"
		BF2::invoke("run Common/Scripts/MainLoop.con");
		BF2::update();
	}
	else
	{
		static bool fullscreen = true;
		switch (lpMsg->message)
		{
		case WM_SYSKEYDOWN:
			// Toggle windowed fullscreen with alt+enter
			// TODO: support for multiple monitors?
			if (lpMsg->wParam == VK_RETURN)
			{
				fullscreen = !fullscreen;
				pWindow->setBorder(!fullscreen);
				pWindow->setClientSize(get_resolution() / (fullscreen ? 1 : 2)); // window size is half the screen resolution
				pWindow->setTopMost(fullscreen);
				pWindow->setPosition({ 0,0 });
			}
		}
	}

	return retVal;
}

// not using auto here to allow tiny optimization on MyLoadLibraryA
IDirect3D9* (WINAPI*pOrigDirect3DCreate9)(UINT) = nullptr;
IDirect3D9* WINAPI MyDirect3DCreate9(UINT sdk_version)
{
	IDirect3D9* d3d = pOrigDirect3DCreate9(sdk_version);
	// Hook the whole Direct3D9 interface via a wrapper class
	return d3d ? new HookedDirect3D9(d3d) : 0;
}

auto pOrigLoadLibraryA = LoadLibraryA;
HMODULE WINAPI MyLoadLibraryA(LPCSTR lpLibFileName)
{
	auto hModule = pOrigLoadLibraryA(lpLibFileName);
	if (!pOrigDirect3DCreate9 && _stricmp(".\\RendDX9.dll", lpLibFileName) == 0)
	{
		pOrigDirect3DCreate9 = IAT::hook_function("Direct3DCreate9", hModule, MyDirect3DCreate9);
	}
	return hModule;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved)
{
	HMODULE hExe = GetModuleHandle(0);
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		
		pOrigPeekMessage = IAT::hook_function("PeekMessageA", hExe, MyPeekMessage);
		pOrigCreateWindowExA = IAT::hook_function("CreateWindowExA", hExe, MyCreateWindowExA);
		pOrigLoadLibraryA = IAT::hook_function("LoadLibraryA", hExe, MyLoadLibraryA);

		break;

	case DLL_PROCESS_DETACH:

		break;
	}
	return true;
}
