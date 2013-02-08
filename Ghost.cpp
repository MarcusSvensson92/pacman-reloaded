#include "stdafx.h"
#include "Ghost.h"

const D3DXVECTOR2 g_ghostSize				   = D3DXVECTOR2(5.50667f, 7.f);
const float		  g_ghostSpeed				   = 20.f;
const float		  g_ghostEatableSpeedReduction = 0.5f;
const float		  g_ghostDeadAlphaValue		   = 0.51f;
const float		  g_ghostDeadSpeedIncrease	   = 3.f;

Ghost::Ghost(GhostAI* ai)
	: Billboard(g_ghostSize, 1.f)
{
	m_ai = ai;

	m_eatableElapsedTime = 0.f;
	m_eatableBlueTime    = 0.f;
	m_eatableTotalTime   = 0.f;
}

Ghost::~Ghost(void)
{
	delete m_ai;
}

void Ghost::MakeEatable(const float blueTime, const float totalTime)
{
	m_ai->SetState(Eatable);
	
	m_eatableElapsedTime = 0.f;
	m_eatableBlueTime    = blueTime;
	m_eatableTotalTime   = totalTime;
}

void Ghost::Kill(void)
{
	m_ai->SetState(Dead);
}

void Ghost::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader, LPCSTR texture, D3DXVECTOR3 pos, D3DXVECTOR3 scale, Node* spawn)
{
	m_ai->SetSpawnNode(spawn);

	Billboard::Init(device, deviceContext, shader, texture, pos, scale);
}

void Ghost::Update(const float dt)
{
	UpdateVelocity(dt);
	UpdateTexture(dt);
	UpdateAlphaValue();
}

void Ghost::InitGFX(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	const std::string eatableTexture1Filename = "Content/Img/eatableghost1.png";
	const std::string eatableTexture2Filename = "Content/Img/eatableghost2.png";
	
	D3DX11CreateShaderResourceViewFromFile(device, eatableTexture1Filename.c_str(), NULL, NULL, &m_eatableTexture1, NULL);
	D3DX11CreateShaderResourceViewFromFile(device, eatableTexture2Filename.c_str(), NULL, NULL, &m_eatableTexture2, NULL);

	Obj3D::InitGFX(device, deviceContext);

	m_roamingTexture = mTexture;
}

void Ghost::UpdateVelocity(const float dt)
{
	D3DXVECTOR3 velocity = m_ai->ComputeDirection() * g_ghostSpeed * dt;
	if (m_ai->GetState() == Eatable) velocity *= g_ghostEatableSpeedReduction;
	if (m_ai->GetState() == Dead)	 velocity *= g_ghostDeadSpeedIncrease;

	mPosition += velocity;

	const float offset = m_ai->ComputeNodeOffset(mPosition);
	if (offset >= 0.f)
	{
		m_ai->UpdateDirection();

		mPosition = m_ai->GetStartNodePosition();
		mPosition += m_ai->ComputeDirection() * offset;
	}
}

void Ghost::UpdateTexture(const float dt)
{
	if (m_ai->GetState() == Eatable)
	{
		m_eatableElapsedTime += dt;

		if (m_eatableElapsedTime >= m_eatableBlueTime)
		{
			const float t = m_eatableElapsedTime - (int)m_eatableElapsedTime;
			if (t > 0.25f && t < 0.5f ||
				t > 0.75f && t < 1.f)
				mTexture = m_eatableTexture1;
			else
				mTexture = m_eatableTexture2;

			if (m_eatableElapsedTime >= m_eatableTotalTime)
			{
				m_ai->SetState(Roaming);
				mTexture = m_roamingTexture;
			}
		}
		else
			mTexture = m_eatableTexture1;
	}
	else
		mTexture = m_roamingTexture;
}

void Ghost::UpdateAlphaValue(void)
{
	switch (m_ai->GetState())
	{
	case Roaming:
		m_alphaValue = 1.f;
		break;
	case Eatable:
		m_alphaValue = 1.f;
		break;
	case Dead:
		m_alphaValue = g_ghostDeadAlphaValue;
		break;
	default:
		m_alphaValue = 1.f;
		break;
	}
}