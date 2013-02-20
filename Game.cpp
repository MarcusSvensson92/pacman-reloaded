#include "stdafx.h"
#include "Game.h"
#include "Blinky.h"
#include "Pinky.h"
#include "Inky.h"
#include "Clyde.h"

Game::Game(void)
{
	gameType = OLD_SCHOOL;
	m_eatenCandy = 0;
	m_totalCandy = 0;
	m_level = 0;
	ChangeLevel(m_level);
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

	initLevel();

	m_eatenCandy = 0;
	m_ghostsEaten = 0;
	
	m_audio.Initialize(hwnd);

	mCamera.UpdateMatrix();

	m_GUIManager.InitScore(m_DeviceContext, m_Device);
	m_GUIManager.InitLife(3, m_DeviceContext, m_Device);
}

void Game::initLevel(void)
{
	mObjList.clear();

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

			m_totalCandy++;
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

			m_totalCandy++;
			//m_lights.AddLight(candy->GetPositionPtr(), SUPERCANDYLIGHT);
		}
		// Add fruit spawnpoint
		if ( ObjectSpawnList[i].Type == FRUIT )
		{	
			//Set pointer to the node where the fruit will spawn.
			m_fruitNode = ObjectSpawnList[i].Node;
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
				ghost = new Ghost(new Pinky(mPlayer.GetNodePtr(), mPlayer.GetNextNodePtr()));
				textureFilename = "Content/Img/pinkghost.png"; 
				m_lights.AddLight(ghost->GetPositionPtr(), GHOSTLIGHT_PINK);
			}

			if (ObjectSpawnList[i].Type == RED_GHOST)
			{	 
				ghost = new Ghost(new Blinky(mPlayer.GetNodePtr(), mPlayer.GetNextNodePtr()));
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
				ghost = new Ghost(new Inky(mPlayer.GetNodePtr(), mPlayer.GetNextNodePtr()));
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
				"Content/Img/Pacman_kelka.png",
				ObjectSpawnList[i].Node->GetPosition(),
				ObjectSpawnList[i].Node);

			m_lights.AddLight(mPlayer.GetPositionPtr(), PACMANLIGHT);
		}
	}
}

void Game::Update(const float dt)
{
	for (std::vector<Obj3D*>::iterator it = mObjList.begin(); it != mObjList.end(); it++)
		(*it)->Update(dt);

	SwitchGameType(dt);

	ChangeView();

	PacManRampage();

	RemoveExpiredFruit();
	
	CountEatenCandy();

	PlayerCollisionGhost();

	// TO DO small animation before reset
	NextLevel();

	PlayerDead();

	UpdateAudio();

	m_GUIManager.UpdateScore(static_cast<std::ostringstream*>( &(std::ostringstream() << mPlayer.GetPoints()) )->str(), m_DeviceContext, m_Device);

	Keyboards();
}

void Game::UpdateAudio()
{
	if (gameType == OLD_SCHOOL)
		m_audio.UpdateListener(D3DXVECTOR3(230, 0, 140), D3DXVECTOR3(0, 1, 0));
	else
		m_audio.UpdateListener(mPlayer.GetPosition(), mCamera.GetLook());

	if (GetAsyncKeyState('m') & 0x8000)
		m_audio.MuteSound();  // <- fungerar inte
}

void Game::Draw()
{
	DrawBegin();

	// Update moving lights
	std::vector<PointLight> tempLights = m_lights.SetMovingLights();
	m_shaders.get("Basic")->SetRawData("gMovingLights", &tempLights[0], sizeof(PointLight)*tempLights.size());

	m_shaders.get("Basic")->SetFloat3("gPlayerPos",mPlayer.GetPosition());
	m_shaders.get("Billboard")->SetFloat3("gPlayerPos",mPlayer.GetPosition());


	if(gameType != FIRST_PERSON)
	{
		m_shaders.get("Billboard")->SetBool("gAnimation",true);
		m_shaders.get("Billboard")->SetFloat3("gMoveVector",mPlayer.GetMoveVector());
		m_shaders.get("Billboard")->SetInt("gFrame",mPlayer.GetFrame());
		m_shaders.get("Billboard")->SetInt("gMaxFrames",mPlayer.GetMaxFrames());
		mPlayer.Draw(m_DeviceContext, mCamera);
		m_shaders.get("Billboard")->SetBool("gAnimation",false);
	}

	// Loop to draw Objects
 	for (int i = 0; i < mObjList.size(); i++) 
 	{
		Candy* x = dynamic_cast<Candy*>(mObjList[i]);
		if (x != NULL)
		{
			if (!x->IsEaten())
				mObjList[i]->Draw(m_DeviceContext, mCamera);
		}
		else
 			mObjList[i]->Draw(m_DeviceContext, mCamera);
 	}
	m_map.Draw(m_DeviceContext, mCamera);

	TurnOffZBuffer();
	m_GUIManager.DrawLife(m_DeviceContext, m_shaders.get("2D"), screenWidth, screenHeight);
	m_GUIManager.DrawScore(m_DeviceContext, m_shaders.get("2D"), screenWidth, screenHeight);
	TurnOnZBuffer();

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

	D3D11_INPUT_ELEMENT_DESC TWODInputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	m_shaders.add("2D", "Content/Shaders/2D.fx", TWODInputDesc, 2);
}

void Game::CameraFollowPlayer()
{
	OnMouseMove();
	mCamera.SetPosition(mPlayer.GetPosition() + D3DXVECTOR3(0, 4.f, 0));
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

	OnMouseMove();
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

void Game::RemoveExpiredFruit()
{
	for ( int i = mObjList.size() - 1; i >= 0; i--)
	{
		Fruit* f = dynamic_cast<Fruit*>(mObjList[i]);
		if (f != NULL)
			if (f->Expired() || f->IsEaten())
			{
				if (f->IsEaten())
					m_audio.PlaySound("Content/Audio/Sounds/pacman_eatfruit.wav");
				mObjList.erase(mObjList.begin() + i);
			}
	}
}
void Game::ChangeLevel(int level)
{
	m_level = level;

	m_ghostblueTime = 8.0f - m_level;
	if (m_ghostblueTime < 0)
		m_ghostblueTime = 0;
	m_ghostweakTime = 11.f - m_level;
	if (m_ghostweakTime < 0)
		m_ghostweakTime = 0;
}
void Game::NextLevel(void)
{
	if (m_eatenCandy == m_totalCandy)
	{
		ChangeLevel(m_level + 1);

		m_eatenCandy = 0;
		m_ghostsEaten = 0;

		for ( int i = mObjList.size() - 1; i >= 0; i--)
		{
			Candy* c = dynamic_cast<Candy*>(mObjList[i]);
			if (c != NULL)
				c->ReSpawn();

			Ghost* g = dynamic_cast<Ghost*>(mObjList[i]);
			if (g != NULL)
				g->Reset();
		}

		mPlayer.ReSpawn();
	}
}

void Game::NewLife(void)
{
	for ( int i = mObjList.size() - 1; i >= 0; i--)
	{
		Ghost* g = dynamic_cast<Ghost*>(mObjList[i]);
		if (g != NULL)
			g->Reset();
	}

	mPlayer.ReSpawn();
}

void Game::CountEatenCandy()
{
	int candyEaten = 0;
	for ( int i = mObjList.size() - 1; i >= 0; i--)
	{
		Candy* x = dynamic_cast<Candy*>(mObjList[i]);
		if (x != NULL)
			if (x->IsEaten())
			{
				candyEaten++;
			}
	}
	m_eatenCandy = candyEaten;
	//Check if pac-man have eaten enough candy to spawn a fruit
	if(m_eatenCandy == 70 || m_eatenCandy == 170)
		SpawnFruit();
}

void Game::SpawnFruit()
{
	if (m_fruitNode->Item != NULL)
		return;

/*
2 fruits per stage, spawn at the same place(under the ghost cage)
The first fruit spawns after eating 70 candy, 2nd fruit after 170 candy
Stage/Name/Points
1: cherry = 100 
2: strawberry = 200
3,4: orange = 500
5,6: apple = 700
7,8: grape = 1000
9,10: ice compot = 2000
11,12: stewed fruit = 3000
13: key = 5000
*/
	LPCSTR texture;
	int points;
	if (m_level == 0)
	{
		texture = "Content/Img/Fruits/cherry.png";
		points	= 100;
	}
	else if (m_level == 1)
	{
		texture = "Content/Img/Fruits/strawberry.png";
		points	= 200;
	}
	else if (m_level < 4)
	{
		texture = "Content/Img/Fruits/orange.png";
		points	= 500;
	}
	else if (m_level < 6)
	{
		texture = "Content/Img/Fruits/apple.png";
		points	= 700;
	}
	else if (m_level < 8)
	{
		texture = "Content/Img/Fruits/grape.png";
		points	= 1000;
	}
	else if (m_level < 10)
	{
		texture = "Content/Img/Fruits/ice_compot.png";
		points	= 2000;
	}
	else if (m_level < 12)
	{
		texture = "Content/Img/Fruits/stewed_fruit.png";
		points	= 3000;
	}
	else
	{
		texture = "Content/Img/Fruits/key.png";
		points	= 5000;
	}


	Fruit* fruit = new Fruit();
	fruit->Init(m_Device, m_DeviceContext,
		m_shaders.get("Billboard"),
		texture,
		m_fruitNode->GetPosition(),
		D3DXVECTOR3(1,1,1), points);
	mObjList.push_back(fruit);

	// Set node item to current fruit.
	m_fruitNode->Item = fruit;
}

void Game::PacManRampage()
{
	if (mPlayer.HasEatenCandy())
		m_audio.PlaySound("Content/Audio/Sounds/pacman_coinin.WAV");

	if (mPlayer.HasEatenSuperCandy())
	{
		for (std::vector<Obj3D*>::iterator it = mObjList.begin(); it != mObjList.end(); it++)
		{
			if (Ghost* ghost = dynamic_cast<Ghost*>((*it)))
			{
				ghost->MakeEatable(m_ghostblueTime, m_ghostweakTime);
				m_audio.PlaySound("Content/Audio/Sounds/pacman_power1.wav");
			}
		}
	}
	else
		m_ghostsEaten = 0;
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
		mCamera.UpdateMatrix();
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
				// Kollar så att pacman inte är död
				if (!mPlayer.IsDead() && !mPlayer.IsHit())
				{
					// Kollar så att spöket inte är död
					if (!x->IsDead())
					{
						// Kollar ifall spöket är farligt
						if (x->IsRoaming())
						{
							// Pacman dör
							mPlayer.Kill();
							m_audio.PlaySound("Content/Audio/Sounds/pacman_death.WAV");
						}
						// Kollar ifall spöket är ätbart
						else if (x->IsEatable())
						{
							// Spöke dör
							x->Kill();
							m_audio.PlaySound("Content/Audio/Sounds/pacman_getghost.WAV");
							//Increase number of eaten ghosts
							m_ghostsEaten++;
							//Award the player with (200 * eaten ghosts) points.
							mPlayer.AddPoints(200 * m_ghostsEaten);
						}
					}
				}
			}
		}
	}
}

void Game::PlayerDead()
{
	if (mPlayer.IsDead())
	{
		m_GUIManager.RemoveLife();
		if (mPlayer.GetLives() > 0)
		{
			NewLife();
		}
		else
		{
			MessageBoxA(0, "YOU SUCK!", 0, 0);
			PostQuitMessage(0); // GAME OVER HERE
		}
	}
}
