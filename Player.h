#pragma once
#include "stdafx.h"
#include "Node.h"

class Player: public Billboard
{
public:

	enum PlayerStatus
	{
		ALIVE,
		DEAD,
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
	~Player(void);

	void Kill();
	void Eat();
	
	D3DXVECTOR3* GetPositionPtr();

	Node*		GetNode();
	Node*		GetNextNode();
	Node**		GetNodePtr();
	Node**		GetNextNodePtr();
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetMoveVector();
	PlayerStatus GetStatus();

	bool HasEatenSuperCandy();

	void Update(D3DXVECTOR3 look, const float dt,bool oldSchoolView,LPCSTR dir );

	void Update(const float dt);

	void OldSchoolControl(LPCSTR dir);

	void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader, LPCSTR texture,D3DXVECTOR3 _pos, Node* _node);

private:

	D3DXVECTOR3		mMoveVector;

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

