#include "stdafx.h"
#include "GhostAI.h"
#include "PathFinding.h"
#include <time.h>
#include <cstdlib>

GhostAI::GhostAI(void)
{
	m_start = NULL;
	m_end	= NULL;
	m_spawn = NULL;

	m_state = Roaming;

	srand(time(NULL));
}

GhostAI::~GhostAI(void) { }

void GhostAI::SetSpawnNode(Node* spawn)
{
	m_start = spawn;
	m_end	= spawn;
	m_spawn = spawn;
}

bool GhostAI::SetState(const GhostState state)
{
	switch (state)
	{
	case Roaming:
		m_state = Roaming;
		return true;
	case Eatable:
		if (m_state == Roaming)
		{
			std::swap(m_start, m_end);

			m_state = Eatable;

			return true;
		}
		break;
	case Dead:
		if (m_state == Eatable)
		{
			m_state = Dead;

			UpdateDirection();
			
			return true;
		}
		break;
	}
	return false;
}

D3DXVECTOR3 GhostAI::ComputeDirection(void) const
{
	D3DXVECTOR3 direction = m_end->GetPosition() - m_start->GetPosition();
	D3DXVec3Normalize(&direction, &direction);
	return direction;
}

float GhostAI::ComputeNodeOffset(const D3DXVECTOR3& position) const
{
	const D3DXVECTOR3 nodeVector = m_end->GetPosition() - m_start->GetPosition();
	const float nodeDistance = D3DXVec3Length(&nodeVector);
	const D3DXVECTOR3 playerVector = position - m_start->GetPosition();
	const float playerDistance = D3DXVec3Length(&playerVector);
	return playerDistance - nodeDistance;
}

void GhostAI::UpdateDirection(void)
{
	switch (m_state)
	{
	case Roaming:
		RoamingBehaviour();
		break;
	case Eatable:
		EatableBehaviour();
		break;
	case Dead:
		DeadBehaviour();
		break;
	}
}

void GhostAI::EatableBehaviour(void)
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

void GhostAI::DeadBehaviour(void)
{
	if (m_end == m_spawn)
	{
		m_state = Roaming;

		m_currentPath.clear();

		UpdateDirection();
	}
	else
	{
		if (m_currentPath.empty() || m_currentPath.front() != m_spawn)
		{
			std::vector<Node*> startPath = Pathfinding::findPath(m_spawn, m_start);
			std::vector<Node*> endPath	 = Pathfinding::findPath(m_spawn, m_end);

			if ((UINT)startPath.size() < (UINT)endPath.size())
			{
				std::swap(m_start, m_end);

				m_currentPath = startPath;
			}
			else
			{
				m_currentPath = endPath;
			}
		}
		else
		{
			m_start = m_end;
			m_end	= m_currentPath.back();

			m_currentPath.pop_back();
		}
	}
}

std::vector<Node*> GhostAI::GetAvailableNodes(void) const
{
	std::vector<Node*> availableNodes;
	if (m_end->Front && m_end->Front != m_start && !m_end->Front->GhostNode) availableNodes.push_back(m_end->Front);
	if (m_end->Back  && m_end->Back  != m_start && !m_end->Back->GhostNode)  availableNodes.push_back(m_end->Back);
	if (m_end->Left  && m_end->Left  != m_start && !m_end->Left->GhostNode)  availableNodes.push_back(m_end->Left);
	if (m_end->Right && m_end->Right != m_start && !m_end->Right->GhostNode) availableNodes.push_back(m_end->Right);
	return availableNodes;
}

bool GhostAI::IsEndNodeInVector(std::vector<Node*> nodes) const
{
	for (std::vector<Node*>::iterator it = nodes.begin(); it != nodes.end(); it++)
		if (m_end == (*it))
			return true;
	return false;
}