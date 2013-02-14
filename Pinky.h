#ifndef PINKY_H
#define PINKY_H

#include "GhostAI.h"
#include "Player.h"

class Pinky : public GhostAI
{
public:
	Pinky(Node** playerStartNode, Node** playerEndNode);
	~Pinky(void);
protected:
	void RoamingBehaviour(void);
private:
	Node** m_playerStartNode;
	Node** m_playerEndNode;
};

#endif