#ifndef INKY_H
#define INKY_H

#include "GhostAI.h"
#include "Player.h"

class Inky : public GhostAI
{
public:
	Inky(Node** playerStartNode, Node** playerEndNode);
	~Inky(void);
protected:
	void RoamingBehaviour(void);
private:
	Node** m_playerStartNode;
	Node** m_playerEndNode;
};

#endif