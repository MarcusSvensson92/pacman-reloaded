#include "stdafx.h"
#include "WinApp.h"

WinApp* g_winApp;

LRESULT CALLBACK msgRouter(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return g_winApp->wndProc(hWnd, msg, wParam, lParam);
}

WinApp::WinApp(HINSTANCE		  hInstance,
			   const std::string& caption,
			   const DWORD		  style,
			   const UINT		  clientWidth,
			   const UINT		  clientHeight)
{
	m_hInstance    = hInstance;
	m_caption	   = caption;
	m_style		   = style;
	m_clientWidth  = clientWidth;
	m_clientHeight = clientHeight;

	g_winApp = this;
}

WinApp::~WinApp(void)
{
}

bool WinApp::init(int cmdShow)
{
	std::string className = "WindowClass";

	// Register window class
	WNDCLASS wc;
	wc.style		 = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc	 = msgRouter;
	wc.cbClsExtra	 = NULL;
	wc.cbWndExtra	 = NULL;
	wc.hInstance	 = m_hInstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = className.c_str();
	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, "RegisterClass failed.", m_caption.c_str(), MB_ICONERROR);
		return false;
	}

	// Calculate window size
	RECT rect = {0, 0, m_clientWidth, m_clientHeight};
	AdjustWindowRect(&rect, m_style, false);
	const int width  = rect.right  - rect.left;
	const int height = rect.bottom - rect.top;

	// Create window handle
	m_hWnd = CreateWindow(className.c_str(), m_caption.c_str(), m_style,
						  CW_USEDEFAULT, CW_USEDEFAULT, width, height,
						  NULL, NULL, m_hInstance, NULL);
	if (!m_hWnd)
	{
		MessageBox(NULL, "CreateWindow failed.", m_caption.c_str(), MB_ICONERROR);
		return false;
	}

	// Display window
	ShowWindow(m_hWnd, cmdShow);
	UpdateWindow(m_hWnd);

	// Init D3D
	m_game.Init(m_hInstance, m_hWnd, true, false, 1.0f, 0.0f); 

	return true;
}

int WinApp::run(void)
{
	m_timer.reset();

	MSG msg = {0};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			m_timer.tick();

			// Add update- and render-stuff here!
			m_game.Update((float)m_timer.getDeltaTime());
			m_game.Render();
		}
	}
	return (int)msg.wParam;
}

LRESULT WinApp::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	// Stop timer while resizing window
	case WM_ENTERSIZEMOVE:
		m_timer.stop();
		break;
	case WM_EXITSIZEMOVE:
		m_timer.start();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}