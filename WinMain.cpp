#include "stdafx.h"
#include "WinApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   PSTR pCmdLine, int nCmdShow)
{
	// Window properties
	const std::string caption	   = "Pacman Reloaded";
	const DWORD		  style		   = WS_OVERLAPPEDWINDOW;
	const UINT		  clientWidth  = 80*16;	// 120*16 for 1920
	const UINT		  clientHeight = 80*9;		// 120*9 for 1080

	// Initialize and run application
	WinApp app(hInstance, caption, style, clientWidth, clientHeight);
	if (!app.init(nCmdShow))
		return EXIT_FAILURE;
	return app.run();
}