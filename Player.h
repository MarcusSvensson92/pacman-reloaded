#pragma once
#include "stdafx.h"
#include "Camera.h"

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
	Player(D3DXVECTOR3 _pos/*, Node _node*/);
	~Player(void);

	void Kill();
	void Eat();
	

/*	Node		GetNode();*/
	D3DXVECTOR3 GetPosition();
	PlayerStatus GetStatus();

	void Update();

	void ChangeDirection(Direction dir);

private:

	D3DXVECTOR3 mPosition;

	Direction mDirection;
	PlayerStatus mStatus;

	bool		mSuperCandy;
/*	Node		mNode;*/

	
	void Move();
};

