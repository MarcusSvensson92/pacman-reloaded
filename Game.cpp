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
	m_level = 1;
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

	mCamera.UpdateMatrix();

}

void Game::initLevel(void)
{
	mObjList.clear();

	m_eatenCandy = 0;
	m_ghostsEaten = 0;
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

	RemoveExpiredFruit();

	CountEatenCandy();

	PlayerCollisionGhost();

	// TO DO small animation before reset
	if (LevelCleared())
		NextLevel();


	if (mPlayer.IsDead())
		NewLife();

	Keyboards();
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

	if(gameType != FIRST_PERSON)
	{
		m_shaders.get("Billboard")->SetBool("gAnimation",true);
		m_shaders.get("Billboard")->SetInt("gFrame",mPlayer.GetFrame());
		m_shaders.get("Billboard")->SetInt("gMaxFrames",mPlayer.GetMaxFrames());
		mPlayer.Draw(m_DeviceContext, mCamera);
		m_shaders.get("Billboard")->SetBool("gAnimation",false);
	}

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

void Game::RemoveExpiredFruit()
{
	for ( int i = mObjList.size() - 1; i >= 0; i--)
	{
		Fruit* f = dynamic_cast<Fruit*>(mObjList[i]);
		if (f != NULL)
			if (f->Expired())
			{
				mObjList.erase(mObjList.begin() + i);
			}
	}
}

bool Game::LevelCleared()
{
	if (m_eatenCandy == 245)
		return true;
	else
		return false;
}

void Game::NextLevel(void)
{
	m_level++;
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
				//Get 1 point for eating a candy
				mPlayer.AddPoints(x->GetPoints());
			}
	}
	m_eatenCandy = candyEaten;
	//Check if pac-man have eaten enough candy to spawn a fruit
	if(m_eatenCandy == 70 || m_eatenCandy == 170)
		SpawnFruit();
}

void Game::SpawnFruit()
{
			Fruit* fruit = new Fruit();
			fruit->Init(m_Device, m_DeviceContext,
				m_shaders.get("Billboard"),
				"Content/Img/Fruits/cherry.png",
				m_fruitNode->GetPosition(),
				D3DXVECTOR3(1,1,1),100);
			mObjList.push_back(fruit);

			// Set node item to current fruit.
			m_fruitNode->Item = fruit;
}

void Game::PacManRampage()
{
	if (mPlayer.HasEatenSuperCandy())
	{
		for (std::vector<Obj3D*>::iterator it = mObjList.begin(); it != mObjList.end(); it++)
		{
			if (Ghost* ghost = dynamic_cast<Ghost*>((*it)))
			{
				ghost->MakeEatable(7.f, 10.f);
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
		// Kollar ifall det �r ett sp�ke
		if (x != NULL)
		{
			// Kollar ifall pacman kolliderar med det
			D3DXVECTOR3 v;
			v = mPlayer.GetPosition() - *x->GetPositionPtr();
			if (D3DXVec3Length(&v) < 5) // 5 = distance
			{
				// Kollar s� att sp�ket inte �r d�tt
				if (!x->IsDead())
				{
					// Kollar ifall sp�ket �r farligt
					if (x->IsRoaming())
					{
						// Pacman d�r
						mPlayer.Kill();
					}
					// Kollar ifall sp�ket �r �tbart
					else if (x->IsEatable())
					{
						// Sp�ke d�r
						x->Kill();
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

void Game::PlayerCollisionFruit()
{
	//for ( int i = 0; i < mObjList.size(); i++)
	//{
	//	Fruit* x = dynamic_cast<Fruit*>(mObjList[i]);
	//	// Kollar ifall det �r en Fruit
	//	if (x != NULL)
	//	{
	//		// Kollar ifall pacman kolliderar med det
	//		D3DXVECTOR3 v;
	//		v = mPlayer.GetPosition() - *x->GetPositionPtr();
	//		if (D3DXVec3Length(&v) < 5) // 5 = distance
	//		{
	//			// Kod f�r att �ta frukten
	//		}
	//	}
	//}
}