#include "stdafx.h"
#include "PinkElephant.h"

const float g_distanceToElephant = 15.f;
const float g_animationSpeed     = 3.f;
const float g_animationPositionY = 1.5f;

PinkElephant::PinkElephant(Player* player)
	: Billboard(D3DXVECTOR2(5.656f, 7.f), 0.7f)
{
	m_player = player;

	m_k = 1.f;
}

PinkElephant::~PinkElephant(void) { }

void PinkElephant::Update(const float dt)
{
	UpdatePosition(dt);
	UpdateAnimation(dt);
}

void PinkElephant::UpdatePosition(const float dt)
{
	D3DXVECTOR3 playerPosition  = m_player->GetPosition();
	D3DXVECTOR3 playerDirection = m_player->GetMoveVector();
	D3DXVec3Normalize(&playerDirection, &playerDirection);

	D3DXVECTOR3 position = playerPosition + playerDirection * g_distanceToElephant;
	mPosition.x = position.x;
	mPosition.z = position.z;
}

void PinkElephant::UpdateAnimation(const float dt)
{
	if (mPosition.y > g_animationPositionY)
		m_k = -1.f;
	else if (mPosition.y < -g_animationPositionY)
		m_k = 1.f;

	mPosition.y += m_k * g_animationSpeed * dt;
}