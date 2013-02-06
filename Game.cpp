#include "stdafx.h"
#include "Game.h"

Game::Game(void)
{
	gameType = NO_CLIP;
}

Game::~Game(void)
{
	for (std::vector<Obj3D*>::iterator it = mObjList.begin(); it != mObjList.end(); it++)
		delete (*it);
}

void Game::Init(HINSTANCE hinstance, HWND hwnd, bool vsync, bool fullscreen, float screenDepth, float screenNear)
{

	d3dApp::Init(hinstance, hwnd, vsync, fullscreen, screenDepth, screenNear);

	initShaders();

	//m_audio.Initialize(hwnd);

	// Init Map and fetch ObjectSpawnList
	std::vector<MapOutput> ObjectSpawnList;
	ObjectSpawnList = m_map.Init(m_Device, m_DeviceContext, m_shaders.get("Basic"), "map.RAW", 28, 31);

	// Loop to spawn Objects
	for (int i = 0; i < ObjectSpawnList.size(); i++) 
	{
		// Add candy
		if ( ObjectSpawnList[i].Type == CANDY )
		{	
			Candy* candy = new Candy();
			candy->Init(m_Device, m_DeviceContext,
				m_shaders.get("Billboard"),
				"Content/Img/candy.png",
				ObjectSpawnList[i].Node->GetPosition(),
				D3DXVECTOR3(1,1,1));
			mObjList.push_back(candy);

			// Set node item to current candy.
			ObjectSpawnList[i].Node->Item = candy;

			//m_lights.AddLight(candy->GetPositionPtr(), CANDYLIGHT);
		}
		// Add candy that looks like supercandy (temp fix)
		if ( ObjectSpawnList[i].Type == SUPER_CANDY )
		{	
			Candy* candy = new Candy();
			candy->Init(m_Device, m_DeviceContext,
				m_shaders.get("Billboard"),
				"Content/Img/supercandy.png",
				ObjectSpawnList[i].Node->GetPosition(),
				D3DXVECTOR3(1,1,1));
			mObjList.push_back(candy);

			// Set node item to current candy.
			ObjectSpawnList[i].Node->Item = candy;

			//m_lights.AddLight(candy->GetPositionPtr(), SUPERCANDYLIGHT);
		}


		if (ObjectSpawnList[i].Type == PINK_GHOST ||
			ObjectSpawnList[i].Type == RED_GHOST ||
			ObjectSpawnList[i].Type == ORANGE_GHOST ||
			ObjectSpawnList[i].Type == TEAL_GHOST)
		{
			Ghost* ghost = new Ghost();

			std::string textureFilename;
			if (ObjectSpawnList[i].Type == PINK_GHOST)
			{	 
				textureFilename = "Content/Img/pinkghost.png"; 
				m_lights.AddLight(ghost->GetPositionPtr(), GHOSTLIGHT_PINK);
			}

			if (ObjectSpawnList[i].Type == RED_GHOST)
			{	 
				textureFilename = "Content/Img/redghost.png"; 
				m_lights.AddLight(ghost->GetPositionPtr(), GHOSTLIGHT_RED);
			}

			if (ObjectSpawnList[i].Type == ORANGE_GHOST)
			{ 
				textureFilename = "Content/Img/orangeghost.png"; 
				m_lights.AddLight(ghost->GetPositionPtr(), GHOSTLIGHT_ORANGE);
			}

			if (ObjectSpawnList[i].Type == TEAL_GHOST)
			{  
				textureFilename = "Content/Img/tealghost.png"; 
				m_lights.AddLight(ghost->GetPositionPtr(), GHOSTLIGHT_TEAL);
			}
			
			ghost->Init(m_Device, m_DeviceContext,
						m_shaders.get("Billboard"),
						textureFilename.c_str(),
						ObjectSpawnList[i].Node->GetPosition(),
						D3DXVECTOR3(1.f, 1.f, 1.f));
			ghost->SetSpawnNode(ObjectSpawnList[i].Node);
			mObjList.push_back(ghost);
		}

		if ( ObjectSpawnList[i].Type == PACMAN)
		{
			mPlayer = Player(ObjectSpawnList[i].Node->GetPosition(), ObjectSpawnList[i].Node);
			m_lights.AddLight(mPlayer.GetPositionPtr(), PACMANLIGHT);
		}
	}

	// Send all candy lights to Shader
	//std::vector<PointLight> tempLights = m_lights.SetCandyLights();
	//m_shaders.get("Basic")->SetRawData("gCandyLights", &tempLights[0], sizeof(PointLight)*241);
}

void Game::Update(const float dt)
{
	switch (gameType)
	{
		case FIRST_PERSON :
			if(mPlayer.GetStatus() == Player::ALIVE)
			{
				CameraFollowPlayer(); 
				mPlayer.Update(mCamera.GetLook());
			}
			break;
		case OLD_SCHOOL :
			OldSchool();
			mCamera.LookAt(D3DXVECTOR3(410, 210, 135), D3DXVECTOR3(200,0,135), D3DXVECTOR3(0,1,0));
			break;
		case NO_CLIP :
			DebugCam(dt);
			break;
	}

	if (GetAsyncKeyState('1') & 0x8000)
	{
		gameType = FIRST_PERSON;
	}
	if (GetAsyncKeyState('2') & 0x8000)
	{
		gameType = OLD_SCHOOL;
	}
	if (GetAsyncKeyState('3') & 0x8000)
	{
		gameType = NO_CLIP;
	}

	if (GetAsyncKeyState('Q') & 0x8000)
	{
		for (std::vector<Obj3D*>::iterator it = mObjList.begin(); it != mObjList.end(); it++)
		{
			if (Ghost* ghost = dynamic_cast<Ghost*>((*it)))
			{
				ghost->ActivateEatable();
			}
		}
	}
	if (GetAsyncKeyState('E') & 0x8000)
	{
		for (std::vector<Obj3D*>::iterator it = mObjList.begin(); it != mObjList.end(); it++)
		{
			if (Ghost* ghost = dynamic_cast<Ghost*>((*it)))
			{
				if (!ghost->IsDead())
					ghost->Kill();
			}
		}
	}

	for (std::vector<Obj3D*>::iterator it = mObjList.begin(); it != mObjList.end(); it++)
		(*it)->Update(dt);

	// Remove eaten candy
	for ( int i = mObjList.size() - 1; i >= 0; i--)
	{
		Candy* x = dynamic_cast<Candy*>(mObjList[i]);
		if (x != NULL)
			if (x->IsEaten())
				mObjList.erase(mObjList.begin() + i);
	}

	d3dApp::Update(dt);
}
void Game::Draw()
{
	DrawBegin();

	// Update moving lights
	std::vector<PointLight> tempLights = m_lights.SetMovingLights();
	m_shaders.get("Basic")->SetRawData("gMovingLights", &tempLights[0], sizeof(PointLight)*tempLights.size());

	// Loop to draw Objects
 	for (int i = 0; i < mObjList.size(); i++) 
 	{
 		mObjList[i]->Draw(m_DeviceContext, mCamera);
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
	mCamera.SetPosition(mPlayer.GetPosition() + D3DXVECTOR3(0, 1.5f, 0));
}

void Game::DebugCam(const float dt )
{
	if (GetAsyncKeyState('W') & 0x8000)
		mCamera.Walk(20.f * dt);
	if (GetAsyncKeyState('S') & 0x8000)
		mCamera.Walk(-20.f * dt);
	if (GetAsyncKeyState('A') & 0x8000)
		mCamera.Strafe(-20.f * dt);
	if (GetAsyncKeyState('D') & 0x8000)
		mCamera.Strafe(20.f * dt);
}

void Game::OldSchool()
{
	if (GetAsyncKeyState('W') & 0x8000)
		mPlayer.OldSchoolControl("UP");
	else if (GetAsyncKeyState('S') & 0x8000)
		mPlayer.OldSchoolControl("DOWN");
	else if (GetAsyncKeyState('A') & 0x8000)
		mPlayer.OldSchoolControl("LEFT");
	else if (GetAsyncKeyState('D') & 0x8000)
		mPlayer.OldSchoolControl("RIGHT");
	else 
		mPlayer.OldSchoolControl("");
}
