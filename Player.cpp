#include "stdafx.h"
#include "Player.h"

/*Node		GetNode()		{return mNode;}*/
D3DXVECTOR3				Player::GetPosition()	{return mPosition;}
Player::PlayerStatus	Player::GetStatus()		{return mStatus;}

Player::Player(){}

Player::Player(D3DXVECTOR3 _pos, Node* _node)
{
	mStatus = ALIVE;
	mPosition = _pos;
	mNode = _node;
}


Player::~Player(void)
{

}

void Player::Update()
{

}

void Player::ChangeDirection( Direction dir )
{
	mDirection = dir;
	switch (dir)
	{
	case FORWARD:
		if (mNode->Front != NULL)
			mNode = mNode->Front;
		break;
	case BACKWARD:
		if (mNode->Back != NULL)
			mNode = mNode->Back;
		break;
	case LEFT:
		if (mNode->Left != NULL)
			mNode = mNode->Left;
		break;
	case RIGHT:
		if (mNode->Right != NULL)
			mNode = mNode->Right;
		break;
	}
	mPosition = mNode->GetPosition();
}
