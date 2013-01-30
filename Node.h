#ifndef _NODE_H_
#define _NODE_H_

class Node
{
public:
	Node(void);
	Node(D3DXVECTOR3 p);
	~Node(void);

	D3DXVECTOR3		GetPosition();

	Node*			Front;
	Node*			Back;
	Node*			Left;
	Node*			Right;
private:
	D3DXVECTOR3		Position;
};

#endif