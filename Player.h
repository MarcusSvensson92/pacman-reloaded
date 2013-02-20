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

	void ReSpawn();
	
	D3DXVECTOR3* GetPositionPtr();

	Node*			GetNode();
	Node*			GetNextNode();
	Node**			GetNodePtr();
	Node**			GetNextNodePtr();
	D3DXVECTOR3		GetPosition();
	D3DXVECTOR3		GetMoveVector();
	PlayerStatus	GetStatus();
	int				GetLives();
	int				GetFrame();
	int				GetMaxFrames();
	bool			IsHit();

	bool HasEatenSuperCandy();
	bool HasEatenCandy();
	bool IsDead();

	void Update(D3DXVECTOR3 look, const float dt,bool oldSchoolView,LPCSTR dir );

	void Update(const float dt);

	void OldSchoolControl(LPCSTR dir);

	void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader, LPCSTR texture,D3DXVECTOR3 _pos, Node* _node);

	void AddPoints(int points);

	int GetPoints() {return mPoints;}

private:

	D3DXVECTOR3		mMoveVector;

	Direction		mDirection,mLastDirection;
	PlayerStatus	mStatus;

	bool			mSuperCandy;
	bool			mCandy;

	int				mLives;
	int				mPoints;

	int				mFrame;
	int				mMaxFrames;
	float			mAnimationSpeed;
	float			mAnimationTimer;

	bool			mHit;

	Node*			mSpawnNode;
	Node*			mNode;
	Node*			mNextNode;

	float			mSpeed;
	float			mDistance;
	float			mDistanceCovered;

	UINT			mImmortalityTimer;
	UINT			mImmortalityMax;

	ID3D11ShaderResourceView*	mKillTexture;
	ID3D11ShaderResourceView*	mAliveTexture;

	
	void Move( const float dt);
	void InputDirection(D3DXVECTOR3 look);
	void ChangeDirection(D3DXVECTOR3 look);

	void CheckDirections();

	void NewDirection(Node* node);
	void Collision(Node* node);
	void SuperCandy();

	void Animation(const float dt);
	void InitGFX(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void SetInitValues();

};

