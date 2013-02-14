#ifndef PINKY_H
#define PINKY_H

#include "GhostAI.h"
#include "Player.h"

class Pinky : public GhostAI
{
public:
	Pinky(Player* player);
	~Pinky(void);
protected:
	void RoamingBehaviour(void);
private:
	Player* m_player;
};

#endif