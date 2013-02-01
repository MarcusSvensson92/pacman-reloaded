#include "stdafx.h"
#include "Pathfinding.h"
#include <algorithm>

namespace Pathfinding
{
	struct pathNode
	{
		bool pathNode::operator> (pathNode n) {return returnF() > n.returnF() ? true : false;}
		bool pathNode::operator< (pathNode n) {return returnF() < n.returnF() ? true : false;}
		bool pathNode::operator== (Node* node) {return n == node ? true : false;}
		bool pathNode::operator!= (Node* node) {return n == node ? false : true;}
		Node* n;
		pathNode* p;
		int h, g;
		int returnF() const { return h+g; }
		pathNode(){}
		pathNode(Node* node, int gValue, int hValue, pathNode* parent): n(node), g(gValue), h(hValue), p(parent) {}
	};

	int hValue(D3DXVECTOR3 v1, D3DXVECTOR3 v2)
	{
		int xDistance = abs(v1.x - v2.x);
		int zDistance = abs(v1.z - v2.z);
		return xDistance + zDistance;
	}

	// ----- READ THIS!!!!!
	// Returns startNode's Neightbor that is closest to the endNode
	//	(Stupid pathfinding)
	Node* ClosestNeighbor(D3DXVECTOR3 endPosition, Node* startNode)
	{
		int h;
		Node* temp;
		temp = startNode->Front; // Om startNode->Front är NULL...
		h = hValue(endPosition, temp->GetPosition()); // ...så får man runtime-error här!
		if (h > hValue(endPosition, startNode->Back->GetPosition()))
		{
			temp = startNode->Back;
			h = hValue(endPosition, temp->GetPosition());
		}
		if (h > hValue(endPosition, startNode->Left->GetPosition()))
		{
			temp = startNode->Left;
			h = hValue(endPosition, temp->GetPosition());
		}
		if (h > hValue(endPosition, startNode->Right->GetPosition()))
		{
			temp = startNode->Right;
			h = hValue(endPosition, temp->GetPosition());
		}
		return temp;
	}
	Node* ClosestNeighbor(Node* endNode, Node* startNode)
	{
		return ClosestNeighbor(endNode->GetPosition(), startNode);
	}

	// ----- READ THIS!!!!!
	// Use pop_back to this output to get the Nodes in right order.
	//	(Smart pathfinding)
	std::vector<Node*> findPath(D3DXVECTOR3 endPosition, Node* startNode)
	{
		std::vector<pathNode> openList, usedList;
		std::vector<pathNode*> closedList;
		openList.push_back(pathNode(startNode, 0, hValue(startNode->GetPosition(), endPosition), 0)); 
		push_heap(openList.begin(), openList.end());
		pop_heap(openList.begin(), openList.end()); 
		usedList.push_back(openList.back());
		closedList.push_back(new pathNode(openList.back()));
		openList.pop_back();
		pathNode* temp; 
		temp = closedList.back(); 
		while (*temp->n != endPosition) 
		{		
			if (temp->n->Front != NULL)
			{
				if (std::find(usedList.begin(), usedList.end(), temp->n->Front) != usedList.end())
				{	
				}
				else
				{
					std::vector<pathNode>::iterator it;
					it = std::find(openList.begin(), openList.end(), temp->n->Front);
					if (it != openList.end())
					{
						if (it->g > temp->g + 10) { it->p = temp; it->g = temp->g + 10; } 
					}
					else
					{
						openList.push_back(pathNode(temp->n->Front, temp->g + 10, hValue(temp->n->Front->GetPosition(), endPosition), temp)); 
						make_heap(openList.begin(),openList.end());
					}
				}
			}
			if (temp->n->Back != NULL)
			{
				if (std::find(usedList.begin(), usedList.end(), temp->n->Back) != usedList.end())
				{	
				}
				else
				{
					std::vector<pathNode>::iterator it;
					it = std::find(openList.begin(), openList.end(), temp->n->Back);
					if (it != openList.end())
					{
						if (it->g > temp->g + 10) { it->p = temp; it->g = temp->g + 10; } 
					}
					else
					{
						openList.push_back(pathNode(temp->n->Back, temp->g + 10, hValue(temp->n->Back->GetPosition(), endPosition), temp)); 
						make_heap(openList.begin(),openList.end());
					}
				}
			}
			if (temp->n->Left != NULL) 
			{
				if (std::find(usedList.begin(), usedList.end(), temp->n->Left) != usedList.end())
				{	
				}
				else
				{
					std::vector<pathNode>::iterator it;
					it = std::find(openList.begin(), openList.end(), temp->n->Left);
					if (it != openList.end())
					{
						if (it->g > temp->g + 10) { it->p = temp; it->g = temp->g + 10; } 
					}
					else
					{
						openList.push_back(pathNode(temp->n->Left, temp->g + 10, hValue(temp->n->Left->GetPosition(), endPosition), temp)); 
						make_heap(openList.begin(),openList.end());
					}
				}
			}
			if (temp->n->Right != NULL) 
			{			
				if (std::find(usedList.begin(), usedList.end(), temp->n->Right) != usedList.end())
				{	
				}
				else
				{
					std::vector<pathNode>::iterator it;
					it = std::find(openList.begin(), openList.end(), temp->n->Right);
					if (it != openList.end())
					{
						if (it->g > temp->g + 10) { it->p = temp; it->g = temp->g + 10; } 
					}
					else
					{
						openList.push_back(pathNode(temp->n->Right, temp->g + 10, hValue(temp->n->Right->GetPosition(), endPosition), temp)); 
						make_heap(openList.begin(),openList.end());
					}
				}
			}

			closedList.push_back(new pathNode(openList.back()));
			usedList.push_back(openList.back());
			openList.pop_back();
			temp = closedList.back(); 
		}

		std::vector<Node*> output;
		while (*temp != startNode)
		{
			output.push_back(temp->n);
			temp = temp->p;
		}

		// Deallocating closed list
		for(std::vector<pathNode*>::iterator it = closedList.begin(); it != closedList.end(); it++)
			delete (*it);

		return output;
	}
	std::vector<Node*> findPath(Node* endNode, Node* startNode)
	{
		return findPath(endNode->GetPosition(), startNode);
	}
}