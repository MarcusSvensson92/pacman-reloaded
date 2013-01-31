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
}


Player::~Player(void)
{

}

//inte klart
void Player::Update()
{
	D3DXVECTOR3 distance;
	distance = mNextNode->GetPosition() - mNode->GetPosition();

	mPosition += distance/100;

	if(mNextNode->GetPosition() == mPosition)
		mNextNode = mNode;
}

//jävligt oklart, håller på å fatta hur det ska gå ihop.
void Player::ChangeDirection( Direction dir ,D3DXVECTOR3 look)
{
	mDirection = dir;
// 	switch (dir)
// 	{
// 	case FORWARD:
// 		if (mNode->Front != NULL)
// 			mNode = mNode->Front;
// 		break;
// 	case BACKWARD:
// 		if (mNode->Back != NULL)
// 			mNode = mNode->Back;
// 		break;
// 	case LEFT:
// 		if (mNode->Left != NULL)
// 			mNode = mNode->Left;
// 		break;
// 	case RIGHT:
// 		if (mNode->Right != NULL)
// 			mNode = mNode->Right;
// 		break;
// 	}

	D3DXVec3Normalize(&look,&look);

	D3DXVECTOR3 lookingAt;//,a,b,c,d,e;

		float a,b,c,d,e;

	if (mNode->Front != NULL)
	{lookingAt = D3DXVECTOR3(-1,0,0)+ look;
	D3DXVec3Normalize(&lookingAt,&lookingAt);
	a = D3DXVec3Dot(&lookingAt,&look);}
	else a = 255;

	if (mNode->Back != NULL)
	{lookingAt = D3DXVECTOR3(1,0,0)+ look;
	D3DXVec3Normalize(&lookingAt,&lookingAt);
	b = D3DXVec3Dot(&lookingAt,&look);}
	else b = 255;

	if (mNode->Left != NULL)
	{lookingAt = D3DXVECTOR3(0,0,1)+ look;
	D3DXVec3Normalize(&lookingAt,&lookingAt);
	c = D3DXVec3Dot(&lookingAt,&look);}
	else c = 255;

	if (mNode->Right != NULL)
	{lookingAt = D3DXVECTOR3(0,0,-1) + look;
	D3DXVec3Normalize(&lookingAt,&lookingAt);
	d = D3DXVec3Dot(&lookingAt,&look);}
	else d = 255;

	std::vector<float> numbers;

	numbers.push_back(a);
	numbers.push_back(b);
	numbers.push_back(c);
	numbers.push_back(d);

	std::sort(numbers.begin(),numbers.end());

	if(a == numbers[0])
	{
		mNextNode = mNode->Front;
	}
	else if(b == numbers[0])
	{
		mNextNode = mNode->Back;
	}
	else if(c == numbers[0])
	{
		mNextNode = mNode->Left;
	}
	else if(d == numbers[0])
	{
		mNextNode = mNode->Right;
	}
}


void Player::Move(Direction dir)
{

}
