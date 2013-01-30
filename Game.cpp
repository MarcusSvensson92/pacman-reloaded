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

	// Init Map and fetch ObjectSpawnList
	std::vector<MapOutput> ObjectSpawnList;
	ObjectSpawnList = m_map.Init(g_Device, g_DeviceContext, "map.RAW", 28, 31);

	// Loop to spawn Objects
	for (int i = 0; i < ObjectSpawnList.size(); i++) 
	{
		if ( ObjectSpawnList[i].Type == CANDY )
		{
			ObjListTest.push_back(Candy(g_Device,g_DeviceContext,ObjectSpawnList[i].Node->Position, D3DXVECTOR3(1,1,1)));
		}
	}

	m_ghost.Init(g_Device, g_DeviceContext, "Content/Img/ghost.png", D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(1.f, 1.f, 1.f));
}

void Game::Update(float dt)
{
	d3dApp::Update(dt);
}
void Game::Draw()
{
	DrawBegin();

	// Loop to draw Objects
	for (int i = 0; i < ObjListTest.size(); i++) 
	{
		ObjListTest[i].Draw(g_DeviceContext, mCamera);
	}
	m_map.Draw(g_DeviceContext, mCamera);
	m_ghost.Draw(g_DeviceContext, mCamera);

	DrawEnd();
}