#include "stdafx.h"
#include "Node.h"


Node::Node(void)
{
}

Node::Node(D3DXVECTOR3 p)
{
	Position = p;
	Front = Back = Left = Right = NULL;
	Item = NULL;
}

Node::~Node(void)
{
}
	
bool Node::operator== (D3DXVECTOR3 v) 
{
	return Position == v ? true : false;
}
bool Node::operator!= (D3DXVECTOR3 v) 
{
	return Position == v ? false : true;
}

D3DXVECTOR3 Node::GetPosition()
{
	return Position;
}