#include "stdafx.h"
#include "Player.h"
#include <algorithm>

Node*					Player::GetNode()		{return mNode;}
Node*					Player::GetNextNode()	{return mNextNode;}
Node**					Player::GetNodePtr()	{return &mNode;}
Node**					Player::GetNextNodePtr(){return &mNextNode;}
D3DXVECTOR3				Player::GetPosition()	{return mPosition;}
D3DXVECTOR3				Player::GetMoveVector() {return mMoveVector;}
Player::PlayerStatus	Player::GetStatus()		{return mStatus;}
D3DXVECTOR3*			Player::GetPositionPtr(){return &mPosition;}
int						Player::GetFrame()		{return mFrame;}
int						Player::GetMaxFrames()	{return mMaxFrames;}

Player::Player(): Billboard(D3DXVECTOR2(7, 7), 1.f) {}

void Player::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader, LPCSTR texture,D3DXVECTOR3 _pos, Node* _node)
{
	mScale = D3DXVECTOR3(1,1,1);
	mPosition = _pos;
	mRotation = D3DXVECTOR3(0,0,0);

	D3DXMatrixScaling(&mTexTransform, 1,1,1);

	mTexturePath = texture;

	mShader = shader;

	InitBuffers(device, deviceContext);
	InitGFX(device,deviceContext);

	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&rotation);
	D3DXMatrixIdentity(&translation);
	D3DXMatrixIdentity(&scaling);


	mStatus = ALIVE;
	mPosition = _pos;
	mNode = _node;
	mNextNode = _node;
	mSpawnNode = _node;
	mPosition = mNode->GetPosition();
	mDirection = PAUSE;
	mDistanceCovered = 0;
	mSpeed = 21;
	mMoveVector = D3DXVECTOR3(0,0,0);
	mSuperCandy = false;

	mFrame = 0;
	mMaxFrames = 4;
	mAnimationSpeed = 0.1f;
	mAnimationTimer = 0;
	mHit = false;
}

void Player::InitGFX(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	if(FAILED(D3DX11CreateShaderResourceViewFromFile(device, mTexturePath.c_str(), 0, 0, &mAliveTexture, 0 )))
	{
		std::string failed = mTexturePath + " Texture Failed";
		MessageBox(0, failed.c_str(), "Fail!", 0);
	}

	if(FAILED(D3DX11CreateShaderResourceViewFromFile(device, "Content/Img/Pacman_dead.png", 0, 0, &mKillTexture, 0 )))
	{
		std::string failed = "Pacman Killtexture Failed";
		MessageBox(0, failed.c_str(), "Fail!", 0);
	}
	mTexture = mAliveTexture;
}

Player::~Player(void)
{

}

void Player::Update(const float dt){}

void Player::Update(D3DXVECTOR3 look, const float dt,bool oldSchoolView,LPCSTR dir )
{
	if(!mHit)
	{
		Move(dt);

		if(!oldSchoolView)
			ChangeDirection(look);
		else
			OldSchoolControl(dir);
	}

	Animation(dt);
	
}

void Player::Animation(const float dt)
{

	mAnimationTimer += dt;

	if(mAnimationTimer > mAnimationSpeed)
	{
		mFrame++;
		mAnimationTimer = 0;
	}

	if(mFrame >= mMaxFrames && mHit)
	{
		mFrame = mMaxFrames;
		mStatus = DEAD;
		return;
	}

	if(mFrame >= mMaxFrames)
		mFrame = 0;
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

bool Player::IsDead()
{
	if (mStatus == DEAD)
		return true;
	else
		return false;
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
	if (mNode != mNextNode && mNode->Item != NULL && !mNode->Item->IsEaten())
	{
		Collision(mNode);
	}

	//kör iterationerna så länge man inte nått noden -> != PAUSE.
	if(mDirection != PAUSE)
	{
		D3DXVec3Normalize(&mMoveVector,&mMoveVector);
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

void Player::ReSpawn()
{
	mStatus = ALIVE;
	mTexture = mAliveTexture;
	
	mNode = mSpawnNode;
	mNextNode = mSpawnNode;
	mPosition = mNode->GetPosition();
	mDirection = PAUSE;
	mDistanceCovered = 0;
	mMoveVector = D3DXVECTOR3(0,0,0);
	mSuperCandy = false;
		
	mFrame = 0;
	mMaxFrames = 4;
	mAnimationSpeed = 0.1f;
	mAnimationTimer = 0;
	mHit = false;
}

void Player::Kill()
{
	// Pacman only die once
	if (!mHit)
	{
		mHit = true;
		mFrame = 0;
		mAnimationSpeed = 0.10f;
		mMaxFrames = 9;
		mTexture = mKillTexture;
	}
}

void Player::NewDirection(Node* node)
{
	mNextNode = node;
	mMoveVector = mNextNode->GetPosition() - mPosition;
	mLastDirection = mDirection;
	mDistance = D3DXVec3Length(&mMoveVector);
}

void Player::AddPoints(int points)
{
	mPoints+=points;
}
