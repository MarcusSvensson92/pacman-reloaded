#include "stdafx.h"
#include "Pinky.h"
#include "PathFinding.h"

Pinky::Pinky(Player* player)
{
	m_player = player;
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
		if (m_start == m_player->GetNextNode())
			m_end = m_player->GetNode();
		else
			m_end = Pathfinding::findPath(m_player->GetNextNode(), m_start).back();

		if (!IsEndNodeInVector(availableNodes))
			m_end = availableNodes[rand() % n];
	}
}