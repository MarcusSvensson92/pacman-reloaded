#ifndef _GAME_H_
#define _GAME_H_

#include "d3dApp.h"
#include "Camera.h"
#include "Candy.h"
#include "Map.h"
#include "Player.h"
#include "Ghost.h"

class Game : public d3dApp
{
public:
	Game(void);
	~Game(void);

	void Init(HINSTANCE, HWND, bool, bool, float, float);
	void Update(float dt);
	void Draw();
private:

	std::vector<Obj3D *> ObjListTest;
	Map m_map;
	Player mPlayer;
	Ghost m_ghost;
	void CameraFollowPlayer();

	Player::Direction mLastKeyDir;
};

#endif

