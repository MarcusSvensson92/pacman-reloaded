#include "stdafx.h"
#include "Clyde.h"

Clyde::Clyde(void)
{
}

Clyde::~Clyde(void) { }

void Clyde::RoamingBehaviour(void)
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
		m_end = availableNodes[rand() % n];
	}
}