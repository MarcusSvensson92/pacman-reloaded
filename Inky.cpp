#include "stdafx.h"
#include "Inky.h"
#include "PathFinding.h"

Inky::Inky(Node** playerStartNode, Node** playerEndNode)
{
	m_playerStartNode = playerStartNode;
	m_playerEndNode	  = playerEndNode;
}

Inky::~Inky(void) { }

void Inky::RoamingBehaviour(void)
{
	std::vector<Node*> availableNodes = GetAvailableNodes();

	m_start = m_end;

	UINT n = (UINT)availableNodes.size();
	if (n == 1)
	{
		m_end = availableNodes[0];
	}
	else if (n > 1)
	{
		if (m_start == *m_playerEndNode)
			m_end = *m_playerStartNode;
		else
			m_end = Pathfinding::findPath(*m_playerEndNode, m_start).back();

		if (!IsEndNodeInVector(availableNodes))
			m_end = availableNodes[rand() % n];
	}
}