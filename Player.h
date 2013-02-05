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
		PAUSE = 0,
		FORWARD = 1,
		BACKWARD = 2,
		LEFT = 3,
		RIGHT = 4,
	};

	Player();
	Player(D3DXVECTOR3 _pos, Node* _node);
	~Player(void);

	void Kill();
	void Eat();
	

	//Node		GetNode();
	D3DXVECTOR3 GetPosition();
	PlayerStatus GetStatus();

	void Update(D3DXVECTOR3 look);

private:

	D3DXVECTOR3		mMoveVector;
	D3DXVECTOR3		mPosition;

	Direction		mDirection,mLastDirection;
	PlayerStatus	mStatus;

	bool			mSuperCandy;

	Node*			mNode;
	Node*			mNextNode;

	UINT			mMoveIterations;
	UINT			mMaxIterations;

	
	void Move();
	void InputDirection(D3DXVECTOR3 look);
	void ChangeDirection(D3DXVECTOR3 look);

	void CheckDirections();

};

