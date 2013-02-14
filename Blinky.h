#ifndef BLINKY_H
#define BLINKY_H

#include "GhostAI.h"
#include "Player.h"

class Blinky : public GhostAI
{
public:
	Blinky(Node** playerStartNode, Node** playerEndNode);
	~Blinky(void);
protected:
	void RoamingBehaviour(void);
private:
	Node** m_playerStartNode;
	Node** m_playerEndNode;
};

#endif