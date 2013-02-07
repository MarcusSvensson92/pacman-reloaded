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
	
	D3DXVECTOR3* GetPositionPtr();

	Node*		GetNode();
	Node*		GetNextNode();
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetMoveVector();
	PlayerStatus GetStatus();

	void Update(D3DXVECTOR3 look, const float dt,bool oldSchoolView,LPCSTR dir );
	void Immortality( const float dt );

	void OldSchoolControl(LPCSTR dir);

private:

	D3DXVECTOR3		mMoveVector;
	D3DXVECTOR3		mPosition;

	Direction		mDirection,mLastDirection;
	PlayerStatus	mStatus;

	bool			mSuperCandy;

	Node*			mNode;
	Node*			mNextNode;

	float			mSpeed;
	float			mDistance;
	float			mDistanceCovered;

	UINT			mImmortalityTimer;
	UINT			mImmortalityMax;

	
	void Move( const float dt);
	void InputDirection(D3DXVECTOR3 look);
	void ChangeDirection(D3DXVECTOR3 look);

	void CheckDirections();

	void NewDirection(Node* node);
	void Collision(Node* node);
	void SuperCandy();
};

