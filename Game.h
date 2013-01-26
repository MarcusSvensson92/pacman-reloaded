#ifndef _GAME_H_
#define _GAME_H_

#include "d3dApp.h"

class Game : public d3dApp
{
public:
	Game(void);
	~Game(void);

	void Init(HINSTANCE, HWND, bool, bool, float, float);
	void Update(float dt);
	void Draw();
};

#endif

