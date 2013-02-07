#include "stdafx.h"
#include "PinkElephant.h"

const float g_distanceToElephant	 = 15.f;
const float g_animationSpeedXZ		 = 4.f;
const float g_animationSpeedY		 = 3.f;
const float g_animationMaxPositionXZ = 10.f;
const float g_animationMaxPositionY  = 1.5f;

PinkElephant::PinkElephant(Player* player)
	: Billboard(D3DXVECTOR2(5.656f, 7.f), 0.7f)
{
	m_player = player;

	m_animationPositionXZ = 0.f;
	m_animationPositionY  = 0.f;

	m_kXZ = 1.f;
	m_kY  = 1.f;

	m_active = true;
}

PinkElephant::~PinkElephant(void) { }

void PinkElephant::Update(const float dt)
{
	if (m_active)
	{
		UpdatePosition(dt);
		UpdateAnimation(dt);
	}
}

void PinkElephant::Draw(ID3D11DeviceContext* deviceContext, Camera camera)
{
	if (m_active)
		Billboard::Draw(deviceContext, camera);
}

void PinkElephant::UpdatePosition(const float dt)
{
	D3DXVECTOR3 playerPosition  = m_player->GetPosition();
	D3DXVECTOR3 playerDirection = m_player->GetMoveVector();
	D3DXVec3Normalize(&playerDirection, &playerDirection);
	D3DXVECTOR3 animationPosition = D3DXVECTOR3(m_animationPositionXZ, m_animationPositionY, m_animationPositionXZ);

	mPosition = playerPosition + playerDirection * g_distanceToElephant + animationPosition;
}

void PinkElephant::UpdateAnimation(const float dt)
{
	// XZ-axle animation
	if (m_animationPositionXZ > g_animationMaxPositionXZ)
		m_kXZ = -1.f;
	if (m_animationPositionXZ < -g_animationMaxPositionXZ)
		m_kXZ = 1.f;

	m_animationPositionXZ += m_kXZ * g_animationSpeedXZ * dt;

	// Y-axle animation
	if (mPosition.y > g_animationMaxPositionY)
		m_kY = -1.f;
	if (mPosition.y < -g_animationMaxPositionY)
		m_kY = 1.f;

	m_animationPositionY += m_kY * g_animationSpeedY * dt;
}