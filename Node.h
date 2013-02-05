#ifndef _NODE_H_
#define _NODE_H_
#include "Candy.h"

class Node
{
public:
	Node(void);
	Node(D3DXVECTOR3 p);
	~Node(void);

	bool operator== (D3DXVECTOR3 v);
	bool operator!= (D3DXVECTOR3 v);

	D3DXVECTOR3		GetPosition();

	Node*			Front;
	Node*			Back;
	Node*			Left;
	Node*			Right;

	Candy*			Item;
private:
	D3DXVECTOR3		Position;
};

#endif