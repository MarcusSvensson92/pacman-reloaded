#ifndef INKY_H
#define INKY_H

#include "GhostAI.h"
#include "Player.h"

class Inky : public GhostAI
{
public:
	Inky(Player* player);
	~Inky(void);
protected:
	void RoamingBehaviour(void);
private:
	Player* m_player;
};

#endif