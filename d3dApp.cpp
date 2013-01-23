#include "stdafx.h"
#include "d3dApp.h"

d3dApp::d3dApp(void)
{
}
d3dApp::~d3dApp(void)
{
}

void d3dApp::Init(HINSTANCE hinstance, HWND hwnd, bool vsync, bool fullscreen, float screenDepth, float screenNear)
{
	RECT rc;
	GetClientRect( hwnd, &rc );
	int screenWidth = rc.right - rc.left;
	int screenHeight = rc.bottom - rc.top;
	// KODA IN SWAPCHAIN OCH SKIT HÄR

	// Create a render target view
	SetRenderTargetView();
	// Create depth stencil texture and the depth stencil view
	SetDepthStencil((float)screenWidth, (float)screenHeight);
	// Setup the viewport
	SetViewPort((float)screenWidth, (float)screenHeight, screenDepth, screenNear);
}

void d3dApp::Update(float dt)
{
}
void d3dApp::Draw()
{
}


void d3dApp::SetRenderTargetView()
{
}
void d3dApp::SetDepthStencil(float width, float heigth)
{
}
void d3dApp::SetViewPort(float width, float heigth, float screenDepth, float screenNear)
{
}