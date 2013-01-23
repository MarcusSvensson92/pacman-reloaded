#ifndef WINAPP_H
#define WINAPP_H

#include "stdafx.h"
#include "Timer.h"

class WinApp
{
public:
	WinApp(HINSTANCE		  hInstance,
		   const std::string& caption,
		   const DWORD		  style,
		   const UINT		  clientWidth,
		   const UINT		  clientHeight);
	~WinApp(void);

	bool init(int cmdShow);
	int  run(void);

	LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	// Handles
	HINSTANCE m_hInstance;
	HWND	  m_hWnd;

	// Properties
	std::string m_caption;
	DWORD		m_style;
	UINT		m_clientWidth;
	UINT		m_clientHeight;

	Timer m_timer;

	// Add D3D-initializer here!
};

#endif