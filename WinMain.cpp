#include "WinApp.h"

// TEST / Anders

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   PSTR pCmdLine, int nCmdShow)
{
	// Window properties
	const std::string caption	   = "Pacman Reloaded";
	const DWORD		  style		   = WS_OVERLAPPEDWINDOW;
	const UINT		  clientWidth  = 800;
	const UINT		  clientHeight = 600;

	// Initialize and run application
	WinApp app(hInstance, caption, style, clientWidth, clientHeight);
	if (!app.init(nCmdShow))
		return EXIT_FAILURE;
	return app.run();
}