#include "stdafx.h"
#include "Game.h"

Game::Game(void)
{
}
Game::~Game(void)
{
	delete m_Graphics;
}

void Game::Init(HINSTANCE hinstance, HWND hwnd, bool vsync, bool fullscreen, float screenDepth, float screenNear)
{
	// Initialize d3d graphic app
	m_Graphics = new d3dApp();
	m_Graphics->Init(hinstance, hwnd, vsync, fullscreen, screenDepth, screenNear);

	// SPACE TO INIT OTHER STUFF

}

void Game::Update(float dt)
{

	// SPACE TO UPDATE STUFF

}
void Game::Draw()
{
	m_Graphics->DrawBegin();

	// SPACE TO DRAW STUFF

	m_Graphics->DrawEnd();
}