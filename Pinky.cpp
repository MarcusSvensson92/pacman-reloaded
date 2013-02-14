#include "stdafx.h"
#include "Pinky.h"
#include "PathFinding.h"

Pinky::Pinky(Node** playerStartNode, Node** playerEndNode)
{
	m_playerStartNode = playerStartNode;
	m_playerEndNode	  = playerEndNode;
}

Pinky::~Pinky(void) { }

void Pinky::RoamingBehaviour(void)
{
	std::vector<Node*> availableNodes = GetAvailableNodes();

	m_start = m_end;

	UINT n = (UINT)availableNodes.size();
	if (n == 1)
	{
		m_end = availableNodes[0];

		if (!m_currentPath.empty())
			m_currentPath.pop_back();
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