#include "stdafx.h"
#include "Game.h"

Game::Game(void)
{
}

Game::~Game(void)
{
	for (std::vector<Obj3D*>::iterator it = ObjListTest.begin(); it != ObjListTest.end(); it++)
		delete (*it);
}

void Game::Init(HINSTANCE hinstance, HWND hwnd, bool vsync, bool fullscreen, float screenDepth, float screenNear)
{
	//mLastKeyDir = Player::Direction::FORWARD;

	d3dApp::Init(hinstance, hwnd, vsync, fullscreen, screenDepth, screenNear);

	initShaders();

	//m_audio.Initialize(hwnd);

	// Init Map and fetch ObjectSpawnList
	std::vector<MapOutput> ObjectSpawnList;
	ObjectSpawnList = m_map.Init(m_Device, m_DeviceContext, m_shaders.get("Basic"), "map.RAW", 28, 31);

	// Loop to spawn Objects
	for (int i = 0; i < ObjectSpawnList.size(); i++) 
	{
		if ( ObjectSpawnList[i].Type == CANDY )
		{	
			Candy* candy = new Candy();
			candy->Init(m_Device, m_DeviceContext, m_shaders.get("Basic"), "Content/Img/white.png", ObjectSpawnList[i].Node->GetPosition(), D3DXVECTOR3(1,1,1));
			ObjListTest.push_back(candy);
		}

		if (ObjectSpawnList[i].Type == PINK_GHOST ||
			ObjectSpawnList[i].Type == RED_GHOST ||
			ObjectSpawnList[i].Type == ORANGE_GHOST ||
			ObjectSpawnList[i].Type == TEAL_GHOST)
		{
			std::string textureFilename;
			if (ObjectSpawnList[i].Type == PINK_GHOST)	 textureFilename = "Content/Img/pinkghost.png";
			if (ObjectSpawnList[i].Type == RED_GHOST)	 textureFilename = "Content/Img/redghost.png";
			if (ObjectSpawnList[i].Type == ORANGE_GHOST) textureFilename = "Content/Img/orangeghost.png";
			if (ObjectSpawnList[i].Type == TEAL_GHOST)   textureFilename = "Content/Img/tealghost.png";

			Ghost* ghost = new Ghost();
			ghost->Init(m_Device, m_DeviceContext,
						m_shaders.get("Billboard"),
						textureFilename.c_str(),
						ObjectSpawnList[i].Node->GetPosition(),
						D3DXVECTOR3(1.f, 1.f, 1.f));
			ghost->SetStartNode(ObjectSpawnList[i].Node);
			ObjListTest.push_back(ghost);
		}

		/*if ( ObjectSpawnList[i].Type == PACMAN)
			mPlayer = Player(ObjectSpawnList[i].Node->GetPosition(), ObjectSpawnList[i].Node);*/
	}
}

void Game::Update(const float dt)
{
	/*if(mPlayer.GetStatus() == Player::ALIVE)
	{CameraFollowPlayer(); mPlayer.Update();}*/

	if (GetAsyncKeyState('W') & 0x8000)
		mCamera.Walk(20.f * dt);
	if (GetAsyncKeyState('S') & 0x8000)
		mCamera.Walk(-20.f * dt);
	if (GetAsyncKeyState('A') & 0x8000)
		mCamera.Strafe(-20.f * dt);
	if (GetAsyncKeyState('D') & 0x8000)
		mCamera.Strafe(20.f * dt);

	for (std::vector<Obj3D*>::iterator it = ObjListTest.begin(); it != ObjListTest.end(); it++)
		(*it)->Update(dt);

	d3dApp::Update(dt);
}
void Game::Draw()
{
	DrawBegin();

	// Loop to draw Objects
 	for (int i = 0; i < ObjListTest.size(); i++) 
 	{
 		ObjListTest[i]->Draw(m_DeviceContext, mCamera);
 	}
	m_map.Draw(m_DeviceContext, mCamera);

	DrawEnd();
}

void Game::initShaders(void)
{
	m_shaders.init(m_Device, m_DeviceContext);
	
	D3D11_INPUT_ELEMENT_DESC basicInputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "DIFFUSE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SPECULAR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	m_shaders.add("Basic", "Content/Shaders/Basic.fx", basicInputDesc, 5);

	D3D11_INPUT_ELEMENT_DESC billboardInputDesc[] = {
		{ "POSITION_W", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE_W",	    0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	m_shaders.add("Billboard", "Content/Shaders/Billboard.fx", billboardInputDesc, 2);
}

void Game::CameraFollowPlayer()
{
	


	if (GetAsyncKeyState('W') & 0x8000)
	{mPlayer.ChangeDirection(mLastKeyDir,mCamera.GetLook());mCamera.SetPosition(mPlayer.GetPosition() + D3DXVECTOR3(0,1.5f,0));}
	else if (GetAsyncKeyState('S') & 0x8000)
		mLastKeyDir = Player::Direction::BACKWARD;
	else if (GetAsyncKeyState('A') & 0x8000)
		mLastKeyDir = Player::Direction::LEFT;
	else if (GetAsyncKeyState('D') & 0x8000)
		mLastKeyDir = Player::Direction::RIGHT;

	

}
