#pragma once
#include "stdafx.h"
#include "Node.h"

class Player
{
public:

	enum PlayerStatus
	{
		ALIVE,
		DEAD,
		IMMORTAL,
	};

	enum Direction
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
	};

	Player();
	Player(D3DXVECTOR3 _pos, Node* _node);
	~Player(void);

	void Kill();
	void Eat();
	

	//Node		GetNode();
	D3DXVECTOR3 GetPosition();
	PlayerStatus GetStatus();

	void Update();

	void ChangeDirection(Direction dir,D3DXVECTOR3 look);

private:

	D3DXVECTOR3 mPosition;

	D3DXVECTOR3 mRight;
	D3DXVECTOR3 mUp;
	D3DXVECTOR3 mLook;

	Direction		mDirection;
	PlayerStatus	mStatus;

	bool			mSuperCandy;
	Node*			mNode;
	Node*			mNextNode;

	
	void Move(Direction dir);
};

