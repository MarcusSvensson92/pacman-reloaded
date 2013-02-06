#ifndef BLINKY_H
#define BLINKY_H

#include "GhostAI.h"
#include "Player.h"

class Blinky : public GhostAI
{
public:
	Blinky(Player* player);
	~Blinky(void);
protected:
	void RoamingBehaviour(void);
private:
	Player* m_player;
};

#endif