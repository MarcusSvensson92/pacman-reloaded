#include "stdafx.h"
#include "Node.h"


Node::Node(void)
{
}

Node::Node(D3DXVECTOR3 p)
{
	Position = p;
	Front = Back = Left = Right = NULL;
}

Node::~Node(void)
{
}

D3DXVECTOR3 Node::GetPosition()
{
	return Position;
}