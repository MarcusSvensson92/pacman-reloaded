#ifndef CLYDE_H
#define CLYDE_H

#include "GhostAI.h"
#include "Player.h"

class Clyde : public GhostAI
{
public:
	Clyde(void);
	~Clyde(void);
protected:
	void RoamingBehaviour(void);
};

#endif