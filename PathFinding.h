#include "stdafx.h"
#include "Node.h"

int hValue(D3DXVECTOR3 v1, D3DXVECTOR3 v2)
{
	int xDistance = abs(v1.x - v2.x);
	int zDistance = abs(v1.z - v2.z);
	if (xDistance > zDistance)
		 return 14*zDistance + 10*(xDistance-zDistance);
	else
		 return 14*xDistance + 10*(zDistance-xDistance);
	//return xDistance + yDistance;
}

Node* ClosestNeighbor(D3DXVECTOR3 Position)
{

}

std::vector<Node*> findPath(D3DXVECTOR3 Position)
{

}