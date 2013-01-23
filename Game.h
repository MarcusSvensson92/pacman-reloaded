#ifndef _GAME_H_
#define _GAME_H_

#include "stdafx.h"
#include "d3dApp.h"

class Game
{
public:
	Game(void);
	~Game(void);

	void Init(HINSTANCE, HWND, bool, bool, float, float);
	void Update(float dt);
	void Draw();

private:
	d3dApp*		m_Graphics;
};

#endif

