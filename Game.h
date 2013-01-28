#ifndef _GAME_H_
#define _GAME_H_

#include "d3dApp.h"
#include "Camera.h"
#include "Candy.h"
#include "Map.h"

class Game : public d3dApp
{
public:
	Game(void);
	~Game(void);

	void Init(HINSTANCE, HWND, bool, bool, float, float);
	void Update(float dt);
	void Draw();
private:
	Obj3D test;
	Map m_map;
};

#endif

