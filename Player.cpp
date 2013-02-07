#include "stdafx.h"
#include "Player.h"
#include <algorithm>

Node*					Player::GetNode()		{return mNode;}
Node*					Player::GetNextNode()	{return mNextNode;}
D3DXVECTOR3				Player::GetPosition()	{return mPosition;}
D3DXVECTOR3				Player::GetMoveVector() {return mMoveVector;}
Player::PlayerStatus	Player::GetStatus()		{return mStatus;}
D3DXVECTOR3*			Player::GetPositionPtr(){return &mPosition;}

Player::Player(){}

Player::Player(D3DXVECTOR3 _pos, Node* _node)
{
	mStatus = ALIVE;
	mPosition = _pos;
	mNode = _node;
	mNextNode = _node;
	mPosition = mNode->GetPosition();
	mDirection = PAUSE;
	mDistanceCovered = 0;
	mSpeed = 3;
	mMoveVector = D3DXVECTOR3(0,0,0);
	mSuperCandy = false;
}


Player::~Player(void)
{

}

void Player::Update(D3DXVECTOR3 look, const float dt,bool oldSchoolView,LPCSTR dir )
{
	Move(dt);

	if(!oldSchoolView)
		ChangeDirection(look);
	else
		OldSchoolControl(dir);
}

void Player::OldSchoolControl(LPCSTR dir)
{
	if(mDirection == PAUSE)
	{
		if (dir == "W")
			mDirection = FORWARD;
		else if (dir == "S")
			mDirection = BACKWARD;
		else if	(dir == "A")
			mDirection = LEFT;
		else if (dir == "D")
			mDirection = RIGHT;

		CheckDirections();
		
		//testa  igen om gamla riktningen fungerar
		if (mDirection == PAUSE); 
		{
			mDirection = mLastDirection;
			CheckDirections();
		}
	}
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

bool Player::HasEatenSuperCandy()
{
	if (mSuperCandy)
	{
		mSuperCandy = false;
		return true;
	}
	else
		return false;
}

void Player::SuperCandy()
{
	mSuperCandy = true;
}

void Player::Collision(Node* node)
{
	node->Item->Eat();

	if(node->Item->IsSuperCandy())
		SuperCandy();
}

void Player::Move( const float dt)
{
	// Äter Godis
	if (mNode != mNextNode && mNode->Item != NULL)
	{
		Collision(mNode);
	}

	//kör iterationerna så länge man inte nått noden -> != PAUSE.
	if(mDirection != PAUSE)
	{
		D3DXVECTOR3 move = mMoveVector*dt*mSpeed;

		mPosition += move;
		mDistanceCovered += D3DXVec3Length(&move);
		mNode = mNextNode; 

		if(mDistanceCovered > mDistance)
		{
			
			mDirection = PAUSE;
			mDistanceCovered =0;
			mPosition = mNode->GetPosition();
		}
	}
}

void Player::CheckDirections()
{
	if (mDirection == FORWARD		&& mNode->Front!=NULL) 
	{
		if (!mNode->Front->GhostNode)
		{NewDirection(mNode->Front);}
	}
	else if (mDirection == BACKWARD	&& mNode->Back!=NULL)
	{
		if (!mNode->Back->GhostNode)
		{NewDirection(mNode->Back);}
	}
	else if (mDirection == LEFT		&& mNode->Left!=NULL)
	{
		if (!mNode->Left->GhostNode)
		{NewDirection(mNode->Left);}
	}
	else if (mDirection == RIGHT	&& mNode->Right!=NULL)
	{
		if (!mNode->Right->GhostNode)
		{NewDirection(mNode->Right);}
	}
	else mDirection = PAUSE;
}

void Player::Kill()
{
	mStatus = DEAD;
}

void Player::NewDirection(Node* node)
{
	mNextNode = node;
	mMoveVector = mNextNode->GetPosition() - mPosition;
	mLastDirection = mDirection;
	mDistance = D3DXVec3Length(&mMoveVector);
}

