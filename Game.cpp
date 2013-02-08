#include "stdafx.h"
#include "Game.h"
#include "Blinky.h"
#include "Pinky.h"
#include "Inky.h"
#include "Clyde.h"
#include "PinkElephant.h"

Game::Game(void)
{
	gameType = OLD_SCHOOL;
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
				D3DXVECTOR3(1,1,1),false);
			mObjList.push_back(candy);

			// Set node item to current candy.
			ObjectSpawnList[i].Node->Item = candy;

		}
		// Add candy that looks like supercandy (temp fix)
		if ( ObjectSpawnList[i].Type == SUPER_CANDY )
		{	
			Candy* candy = new Candy();
			candy->Init(m_Device, m_DeviceContext,
				m_shaders.get("Billboard"),
				"Content/Img/supercandy.png",
				ObjectSpawnList[i].Node->GetPosition(),
				D3DXVECTOR3(1,1,1),true);
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
			Ghost* ghost;

			std::string textureFilename;
			if (ObjectSpawnList[i].Type == PINK_GHOST)
			{	 
				ghost = new Ghost(new Pinky(&mPlayer));
				textureFilename = "Content/Img/pinkghost.png"; 
				m_lights.AddLight(ghost->GetPositionPtr(), GHOSTLIGHT_PINK);
			}

			if (ObjectSpawnList[i].Type == RED_GHOST)
			{	 
				ghost = new Ghost(new Blinky(&mPlayer));
				textureFilename = "Content/Img/redghost.png"; 
				m_lights.AddLight(ghost->GetPositionPtr(), GHOSTLIGHT_RED);
			}

			if (ObjectSpawnList[i].Type == ORANGE_GHOST)
			{ 
				ghost = new Ghost(new Clyde());
				textureFilename = "Content/Img/orangeghost.png"; 
				m_lights.AddLight(ghost->GetPositionPtr(), GHOSTLIGHT_ORANGE);
			}

			if (ObjectSpawnList[i].Type == TEAL_GHOST)
			{  
				ghost = new Ghost(new Inky(&mPlayer));
				textureFilename = "Content/Img/tealghost.png"; 
				m_lights.AddLight(ghost->GetPositionPtr(), GHOSTLIGHT_TEAL);
			}
			
			ghost->Init(m_Device, m_DeviceContext,
						m_shaders.get("Billboard"),
						textureFilename.c_str(),
						ObjectSpawnList[i].Node->GetPosition(),
						D3DXVECTOR3(1.f, 1.f, 1.f),
						ObjectSpawnList[i].Node);
			mObjList.push_back(ghost);
		}

		if (ObjectSpawnList[i].Type == PACMAN)
		{
			mPlayer.Init(m_Device, m_DeviceContext,
				m_shaders.get("Billboard"),
				"Content/Img/Pacman.png",
				ObjectSpawnList[i].Node->GetPosition(),
				ObjectSpawnList[i].Node);

			m_lights.AddLight(mPlayer.GetPositionPtr(), PACMANLIGHT);
		}
	}

	PinkElephant* pinkElephant = new PinkElephant(&mPlayer);
	pinkElephant->Init(m_Device, m_DeviceContext,
						m_shaders.get("Billboard"),
						"Content/Img/pinkelephant.png",
						D3DXVECTOR3(0.f, 0.f, 0.f),
						D3DXVECTOR3(1.f, 1.f, 1.f));
	mObjList.push_back(pinkElephant);

}

void Game::Update(const float dt)
{
	for (std::vector<Obj3D*>::iterator it = mObjList.begin(); it != mObjList.end(); it++)
		(*it)->Update(dt);

	SwitchGameType(dt);

	ChangeView();

	PacManRampage();

	Trams();

	RemoveEatenCandy();

	PlayerCollisionGhost();

	

// 		D3D11_BLEND SrcBlend.;
// 
// 		D3D10_BLEND_OP BlendOp.;



	//if(PlayerCollisionGhost() && mPlayer.GetStatus() != Player::PlayerStatus::IMMORTAL)
	//mPlayer.Kill();

	d3dApp::Update(dt);
}
void Game::Draw()
{
	DrawBegin();

	// Update moving lights
	std::vector<PointLight> tempLights = m_lights.SetMovingLights();
	m_shaders.get("Basic")->SetRawData("gMovingLights", &tempLights[0], sizeof(PointLight)*tempLights.size());

	m_shaders.get("Basic")->SetFloat3("gPlayerPos",mPlayer.GetPosition());
	m_shaders.get("Billboard")->SetFloat3("gPlayerPos",mPlayer.GetPosition());

	// Loop to draw Objects
 	for (int i = 0; i < mObjList.size(); i++) 
 	{
 		mObjList[i]->Draw(m_DeviceContext, mCamera);
 	}
	m_map.Draw(m_DeviceContext, mCamera);

	if(gameType == OLD_SCHOOL)
		mPlayer.Draw(m_DeviceContext, mCamera);

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
		mLastKey = "W";
	else if (GetAsyncKeyState('S') & 0x8000)
		mLastKey = "S";
	else if (GetAsyncKeyState('A') & 0x8000)
		mLastKey = "A";
	else if (GetAsyncKeyState('D') & 0x8000)
		mLastKey = "D";
	else 
		mLastKey = "";
}

void Game::Trams()
{
	if (GetAsyncKeyState('P') & 0x8000)
	{
		for (std::vector<Obj3D*>::iterator it = mObjList.begin(); it != mObjList.end(); it++)
		{
			if (PinkElephant* pinkElephant = dynamic_cast<PinkElephant*>((*it)))
			{
				pinkElephant->Activate();
			}
		}
	}
	if (GetAsyncKeyState('L') & 0x8000)
	{
		for (std::vector<Obj3D*>::iterator it = mObjList.begin(); it != mObjList.end(); it++)
		{
			if (PinkElephant* pinkElephant = dynamic_cast<PinkElephant*>((*it)))
			{
				pinkElephant->Deactivate();
			}
		}
	}
}

void Game::RemoveEatenCandy()
{
	for ( int i = mObjList.size() - 1; i >= 0; i--)
	{
		Candy* x = dynamic_cast<Candy*>(mObjList[i]);
		if (x != NULL)
			if (x->IsEaten())
				mObjList.erase(mObjList.begin() + i);
	}
}

void Game::PacManRampage()
{
	if (mPlayer.HasEatenSuperCandy())
	{
		for (std::vector<Obj3D*>::iterator it = mObjList.begin(); it != mObjList.end(); it++)
		{
			if (Ghost* ghost = dynamic_cast<Ghost*>((*it)))
			{
				ghost->MakeEatable(3.f, 5.f);
			}
		}
	}
}

void Game::ChangeView()
{
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
}

void Game::SwitchGameType( const float dt )
{
	switch (gameType)
	{
	case FIRST_PERSON :
 		if(!(mPlayer.GetStatus() == Player::DEAD))
 		{
			CameraFollowPlayer(); 
			mPlayer.Update(mCamera.GetLook(),dt,false,"");
		}
		break;
	case OLD_SCHOOL :
		if(!(mPlayer.GetStatus() == Player::DEAD))
		{
			OldSchool();
			mPlayer.Update(mCamera.GetLook(),dt,true,mLastKey);
			mCamera.LookAt(D3DXVECTOR3(410, 210, 135), D3DXVECTOR3(200,0,135), D3DXVECTOR3(0,1,0));
		}
		break;
	case NO_CLIP :
		DebugCam(dt);
		break;
	}
}

void Game::PlayerCollisionGhost()
{
	for ( int i = 0; i < mObjList.size(); i++)
	{
		Ghost* x = dynamic_cast<Ghost*>(mObjList[i]);
		// Kollar ifall det är ett spöke
		if (x != NULL)
		{
			// Kollar ifall pacman kolliderar med det
			D3DXVECTOR3 v;
			v = mPlayer.GetPosition() - *x->GetPositionPtr();
			if (D3DXVec3Length(&v) < 5) // 5 = distance
			{
				// Kollar så att spöket inte är dött
				if (!x->IsDead())
				{
					// Kollar ifall spöket är farligt
					if (x->IsRoaming())
					{
						// Pacman dör
						mPlayer.Kill();
					}
					// Kollar ifall spöket är ätbart
					else if (x->IsEatable())
					{
						// Spöke dör
						x->Kill();
					}
				}
			}
		}
	}
}

void Game::PlayerCollisionFruit()
{
	//for ( int i = 0; i < mObjList.size(); i++)
	//{
	//	Fruit* x = dynamic_cast<Fruit*>(mObjList[i]);
	//	// Kollar ifall det är en Fruit
	//	if (x != NULL)
	//	{
	//		// Kollar ifall pacman kolliderar med det
	//		D3DXVECTOR3 v;
	//		v = mPlayer.GetPosition() - *x->GetPositionPtr();
	//		if (D3DXVec3Length(&v) < 5) // 5 = distance
	//		{
	//			// Kod för att äta frukten
	//		}
	//	}
	//}
}