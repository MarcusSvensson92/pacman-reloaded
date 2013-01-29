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
	mLastKeyDir = Player::Direction::FORWARD;

	d3dApp::Init(hinstance, hwnd, vsync, fullscreen, screenDepth, screenNear);

	// Init Map and fetch ObjectSpawnList
	std::vector<MapOutput> ObjectSpawnList;
	ObjectSpawnList = m_map.Init(g_Device, g_DeviceContext, "map.RAW", 28, 31);

	// Loop to spawn Objects
	for (int i = 0; i < ObjectSpawnList.size(); i++) 
	{
		if ( ObjectSpawnList[i].Type == CANDY )
		{	

			ObjListTest.push_back(new Candy (g_Device,g_DeviceContext,ObjectSpawnList[i].Node->Position, D3DXVECTOR3(1,1,1)));

		}

		if ( ObjectSpawnList[i].Type == PACMAN)
			mPlayer = Player(ObjectSpawnList[i].Node->Position);
	}
	
}

void Game::Update(float dt)
{
	if(mPlayer.GetStatus() == Player::ALIVE)
	CameraFollowPlayer();

	d3dApp::Update(dt);
}
void Game::Draw()
{
	DrawBegin();

	// Loop to draw Objects
 	for (int i = 0; i < ObjListTest.size(); i++) 
 	{
 		ObjListTest[i]->Draw(g_DeviceContext, mCamera);
 	}
	m_map.Draw(g_DeviceContext, mCamera);

	DrawEnd();
}

void Game::CameraFollowPlayer()
{
	mCamera.SetPosition(mPlayer.GetPosition());


	if (GetAsyncKeyState('W') & 0x8000)
		mLastKeyDir = Player::Direction::FORWARD;
	else if (GetAsyncKeyState('S') & 0x8000)
		mLastKeyDir = Player::Direction::BACKWARD;
	else if (GetAsyncKeyState('A') & 0x8000)
		mLastKeyDir = Player::Direction::LEFT;
	else if (GetAsyncKeyState('D') & 0x8000)
		mLastKeyDir = Player::Direction::RIGHT;

	mPlayer.ChangeDirection(mLastKeyDir);

}
