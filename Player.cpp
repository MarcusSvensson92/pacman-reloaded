#include "stdafx.h"
#include "Player.h"
#include <algorithm>

/*Node		GetNode()		{return mNode;}*/
D3DXVECTOR3				Player::GetPosition()	{return mPosition;}
Player::PlayerStatus	Player::GetStatus()		{return mStatus;}

Player::Player(){}

Player::Player(D3DXVECTOR3 _pos, Node* _node)
{
	mStatus = ALIVE;
	mPosition = _pos;
	mNode = _node;
	mNextNode = _node;
	mPosition = mNode->GetPosition();
	mDirection = PAUSE;
	mMoveIterations = 0;
	mMaxIterations = 15;
	mMoveVector = D3DXVECTOR3(0,0,0);
	
}


Player::~Player(void)
{

}

void Player::Update(D3DXVECTOR3 look)
{
	Move();
	ChangeDirection(look);
}

void Player::InputDirection(D3DXVECTOR3 look)
{
	//	 NODE STRUCTURE			-- skiljer sig mot deklarationen i Map. z är motsatt?
	// z
	// ^
	// |      Right
	// |       o				Up		= -x 
	// |       |				Back	= +x 
	// |  Up o-o-o Back			Right	= +z 
	// |       |				Left	= -z 
	// |       o 
	// |     Left
	//-|-----------------> x

	if(look.x < 0 && look.z > -0.5f && look.z < 0.5f)
		mDirection = FORWARD;
	else if (look.x > 0 && look.z < 0.5f && look.z > -0.5f)
		mDirection = BACKWARD;

	else if(look.z < 0 && look.x < 0.5f && look.x > -0.5f)
		mDirection = LEFT;
	else if(look.z > 0 && look.x > -0.5f && look.x < 0.5f)
		mDirection = RIGHT;
}

void Player::ChangeDirection(D3DXVECTOR3 look)
{
	bool found;

	//PAUSE innebär att man nått en ny nod, då kan man byta
	if(mDirection == PAUSE)
	{
		D3DXVec3Normalize(&look,&look);

		InputDirection(look);

		CheckDirections();
		
		//testa  igen om gamla riktningen fungerar
		if (mDirection == PAUSE); 
		{
			mDirection = mLastDirection;
			CheckDirections();
		}
	}
	
}


void Player::Move()
{
	// Äter Godis
	if (mNode != mNextNode && mNode->Item != NULL)
	{
		mNode->Item->Eat();
	}

	//kör iterationerna så länge man inte nått noden -> != PAUSE.
	if(mDirection != PAUSE)
	{
		mPosition += mMoveVector/mMaxIterations;

		mNode = mNextNode; 

		mMoveIterations++;

		if(mMoveIterations > mMaxIterations)
		{
			mDirection = PAUSE;
			mMoveIterations =0;
			mPosition = mNode->GetPosition();
		}
	}
}

void Player::CheckDirections()
{
	if (mDirection == FORWARD		&& mNode->Front!=NULL)
	{mNextNode = mNode->Front;	mMoveVector = mNextNode->GetPosition() - mPosition;mLastDirection = mDirection;}
	else if (mDirection == BACKWARD	&& mNode->Back!=NULL)
	{mNextNode = mNode->Back;	mMoveVector = mNextNode->GetPosition() - mPosition;mLastDirection = mDirection;}
	else if (mDirection == LEFT		&& mNode->Left!=NULL)
	{mNextNode = mNode->Left;	mMoveVector = mNextNode->GetPosition() - mPosition;mLastDirection = mDirection;}
	else if (mDirection == RIGHT	&& mNode->Right!=NULL)
	{mNextNode = mNode->Right;	mMoveVector = mNextNode->GetPosition() - mPosition;mLastDirection = mDirection;}
	else mDirection = PAUSE;
}
