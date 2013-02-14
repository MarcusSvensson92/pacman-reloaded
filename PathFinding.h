#include "stdafx.h"
#include "Node.h"

namespace Pathfinding
{
	// ----- READ THIS!!!!!
	// Returns startNode's Neightbor that is closest to the endNode
	//	(Stupid pathfinding)
	Node* ClosestNeighbor(D3DXVECTOR3 endPosition, Node* startNode);
	Node* ClosestNeighbor(Node* endNode, Node* startNode);

	// ----- READ THIS!!!!!
	// Use pop_back to this output to get the Nodes in right order.
	//	(Smart pathfinding)
	std::vector<Node*> findPath(D3DXVECTOR3 endPosition, Node* startNode);
	std::vector<Node*> findPath(Node* endNode, Node* startNode);
}