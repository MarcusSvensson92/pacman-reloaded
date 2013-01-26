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

	// SPACE TO INIT OTHER STUFF

}

void Game::Update(float dt)
{
	d3dApp::Update(dt);

	// SPACE TO UPDATE STUFF

}
void Game::Draw()
{
	DrawBegin();
	
	// SPACE TO DRAW STUFF

	DrawEnd();
}
