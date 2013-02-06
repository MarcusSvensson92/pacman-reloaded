#ifndef GHOSTAI_H
#define GHOSTAI_H

#include "Node.h"

enum GhostState
{
	Roaming,
	Eatable,
	Dead
};

class GhostAI
{
public:
	GhostAI(void);
	~GhostAI(void);

	GhostState GetState(void) const { return m_state; }
	D3DXVECTOR3 GetStartNodePosition(void) const { return m_start->GetPosition(); }

	void SetSpawnNode(Node* spawn);
	bool SetState(const GhostState state);

	D3DXVECTOR3 ComputeDirection(void) const;
	float ComputeNodeOffset(const D3DXVECTOR3& position) const;

	void UpdateDirection(void);
protected:
	virtual void RoamingBehaviour(void) = 0;
	virtual void EatableBehaviour(void);
	virtual void DeadBehaviour(void);

	std::vector<Node*> GetAvailableNodes(void) const;
	bool IsEndNodeInVector(std::vector<Node*> nodes) const;

	Node* m_start;
	Node* m_end;
	Node* m_spawn;

	std::vector<Node*> m_currentPath;
private:
	GhostState m_state;
};

#endif