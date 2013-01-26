#include "stdafx.h"
#include "Game.h"

Game::Game(void)
{
}

Game::~Game(void)
{
}

void Game::Init(HINSTANCE hinstance, HWND hwnd, bool vsync, bool fullscreen, float screenDepth, float screenNear)
{
	d3dApp::Init(hinstance, hwnd, vsync, fullscreen, screenDepth, screenNear);

	test = Obj3D(g_Device,g_DeviceContext,D3DXVECTOR3(0,0,0), D3DXVECTOR3(1,1,1));
}

void Game::Update(float dt)
{
	d3dApp::Update(dt);
}
void Game::Draw()
{
	DrawBegin();
	
	test.Draw(g_DeviceContext, mCamera);

	DrawEnd();
}