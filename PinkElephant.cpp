#include "stdafx.h"
#include "PinkElephant.h"

const float       g_distanceToElephant   = 10.f;
const D3DXVECTOR3 g_animationSpeed		 = D3DXVECTOR3(7.f, 4.f, 7.f);
const D3DXVECTOR3 g_animationMaxPosition = D3DXVECTOR3(10.f, 1.5, 10.f);

PinkElephant::PinkElephant(Player* player)
	: Billboard(D3DXVECTOR2(5.656f, 7.f), 0.7f)
{
	m_player = player;

	m_animationPosition = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_animationK		= D3DXVECTOR3(1.f, 1.f, 1.f);

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

	D3DXVECTOR3 animationPosition = m_animationPosition;
	if (playerDirection.x > 0.f)
		animationPosition.z *= -1.f;
	if (playerDirection.x < 0.f)
		animationPosition.x *= -1.f;
	if (playerDirection.z < 0.f)
	{
		animationPosition.x *= -1.f;
		animationPosition.z *= -1.f;
	}

	mPosition = playerPosition + playerDirection * g_distanceToElephant + animationPosition;
}

void PinkElephant::UpdateAnimation(const float dt)
{
	// XZ-axle animation
	if (m_animationPosition.x >= g_animationMaxPosition.x)
		m_animationK.x = -1.f;
	if (m_animationPosition.x <= -g_animationMaxPosition.x)
		m_animationK.x = 1.f;
	if (m_animationPosition.z >= g_animationMaxPosition.z)
		m_animationK.z = -1.f;
	if (m_animationPosition.z <= -g_animationMaxPosition.z)
		m_animationK.z = 1.f;

	m_animationPosition.x += m_animationK.x * g_animationSpeed.x * dt;
	m_animationPosition.z += m_animationK.z * g_animationSpeed.z * dt;

	// Y-axle animation
	if (m_animationPosition.y > g_animationMaxPosition.y)
		m_animationK.y = -1.f;
	if (m_animationPosition.y < -g_animationMaxPosition.y)
		m_animationK.y = 1.f;

	m_animationPosition.y += m_animationK.y * g_animationSpeed.y * dt;
}