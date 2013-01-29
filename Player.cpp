#include "stdafx.h"
#include "Player.h"

/*Node		GetNode()		{return mNode;}*/
D3DXVECTOR3				Player::GetPosition()	{return mPosition;}
Player::PlayerStatus	Player::GetStatus()		{return mStatus;}

Player::Player(){}

Player::Player(D3DXVECTOR3 _pos/*, Node _node*/)
{
	mStatus = ALIVE;
	mPosition = _pos;
/*	mNode = _node;*/
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
}
