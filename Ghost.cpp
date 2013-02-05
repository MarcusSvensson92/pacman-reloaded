#include "stdafx.h"
#include "Ghost.h"
#include <time.h>
#include <cstdlib>
#include "PathFinding.h"

const D3DXVECTOR2 g_ghostSize				   = D3DXVECTOR2(7.f, 7.f);
const float		  g_ghostSpeed				   = 20.f;
const float		  g_ghostEatableBlueTime	   = 3.f;
const float		  g_ghostEatableTotalTime	   = 5.f;
const float		  g_ghostEatableSpeedReduction = 0.5f;
const float		  g_ghostEatedAlphaValue	   = 0.51f;
const float		  g_ghostEatedSpeedIncrease	   = 3.f;

Ghost::Ghost(void)
{
	m_start = NULL;
	m_end   = NULL;

	m_state = Roaming;

	m_elapsedTime = 0.f;

	srand(time(NULL));
}

Ghost::~Ghost(void)
{
}

void Ghost::SetSpawnNode(Node* node)
{
	m_spawn = node;
	m_start = node;
	m_end	= NULL;

	ComputeNewNodes();
}

void Ghost::ActivateEatable(void)
{
	if (m_state == Roaming)
		std::swap(m_start, m_end);
	m_elapsedTime = 0.f;
	m_state = Eatable;
}

void Ghost::ActivateEated(void)
{
	m_state = Eated;

	ComputeNewNodes();
}

void Ghost::Update(const float dt)
{
	UpdateVelocity(dt);

	if (m_state == Eatable)
		m_elapsedTime += dt;
}

void Ghost::Draw(ID3D11DeviceContext* deviceContext, Camera camera)
{
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	D3DXMatrixTranslation(&translation, mPosition.x, mPosition.y, mPosition.z);
	world = translation;

	mShader->SetMatrix("gWorld", world);
	mShader->SetMatrix("gViewProj", camera.ViewProj());
	mShader->SetFloat3("gCameraPositionW", camera.GetPosition());

	if (m_state == Eated) mShader->SetFloat("gAlphaValue", g_ghostEatedAlphaValue);
	else				  mShader->SetFloat("gAlphaValue", 1.f);

	UpdateTexture();

	mVBuffer->Apply();
	mShader->Apply(0);

	deviceContext->Draw(1, 0);
}

void Ghost::InitBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	struct BillboardVertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 size;
	};

	BillboardVertex vertex;
	vertex.position = D3DXVECTOR3(0.f, 0.f, 0.f);
	vertex.size		= g_ghostSize;

	BUFFER_INIT_DESC initDesc;
	initDesc.ElementSize = sizeof(BillboardVertex);
	initDesc.InitData	 = &vertex;
	initDesc.NumElements = 1;
	initDesc.Type		 = VERTEX_BUFFER;
	initDesc.Usage		 = BUFFER_DEFAULT;

	mVBuffer = new Buffer();
	if(FAILED(mVBuffer->Init(device, deviceContext, initDesc)))
	{
		MessageBox(0, "Failed to initialize vertex buffer in Obj3D.cpp", "Fail!", 0);
	}
}

void Ghost::InitGFX(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	const std::string eatableTexture1Filename = "Content/Img/eatableghost1.png";
	const std::string eatableTexture2Filename = "Content/Img/eatableghost2.png";
	
	D3DX11CreateShaderResourceViewFromFile(device, eatableTexture1Filename.c_str(), NULL, NULL, &m_eatableTexture1, NULL);
	D3DX11CreateShaderResourceViewFromFile(device, eatableTexture2Filename.c_str(), NULL, NULL, &m_eatableTexture2, NULL);

	Obj3D::InitGFX(device, deviceContext);
}

void Ghost::UpdateVelocity(const float dt)
{
	D3DXVECTOR3 direction = m_end->GetPosition() - m_start->GetPosition();
	D3DXVec3Normalize(&direction, &direction);

	D3DXVECTOR3 velocity = direction * g_ghostSpeed * dt;
	if (m_state == Eatable) velocity *= g_ghostEatableSpeedReduction;
	if (m_state == Eated)	velocity *= g_ghostEatedSpeedIncrease;

	mPosition += velocity;

	if (IsEndNodePassed())
	{
		if (m_end == m_spawn && m_state == Eated)
		{
			m_currentPath.clear();
			m_state = Roaming;
		}

		ComputeNewNodes();
	}
}

void Ghost::UpdateTexture(void)
{
	if (m_state == Eatable)
	{
		if (m_elapsedTime >= g_ghostEatableBlueTime)
		{
			const float t = m_elapsedTime - (int)m_elapsedTime;
			if (t > 0.25f && t < 0.5f ||
				t > 0.75f && t < 1.f)
				mShader->SetResource("gTexture", m_eatableTexture1);
			else
				mShader->SetResource("gTexture", m_eatableTexture2);

			if (m_elapsedTime >= g_ghostEatableTotalTime)
			{
				m_state = Roaming;
				mShader->SetResource("gTexture", mTexture);
			}
		}
		else
			mShader->SetResource("gTexture", m_eatableTexture1);
	}
	else
		mShader->SetResource("gTexture", mTexture);
}

bool Ghost::IsEndNodePassed(void)
{
	const D3DXVECTOR3 nodeVector = m_end->GetPosition() - m_start->GetPosition();
	const float nodeDistance = D3DXVec3Length(&nodeVector);
	const D3DXVECTOR3 playerVector = mPosition - m_start->GetPosition();
	const float playerDistance = D3DXVec3Length(&playerVector);

	if (playerDistance >= nodeDistance)
		return true;
	else
		return false;
}

void Ghost::ComputeNewNodes(void)
{
	if (m_state == Eated)
	{
		if (m_currentPath.empty())
		{
			std::vector<Node*> startPath = Pathfinding::findPath(m_spawn, m_start);
			std::vector<Node*> endPath	 = Pathfinding::findPath(m_spawn, m_end);

			if ((UINT)startPath.size() < (UINT)endPath.size())
			{
				m_currentPath = startPath;
				std::swap(m_start, m_end);
			}
			else
			{
				m_currentPath = endPath;
			}
		}
		else
		{
			m_start = m_end;
			m_end   = m_currentPath.back();
			m_currentPath.pop_back();
		}
	}
	else
	{
		if (!m_end)
			m_end = m_start;

		std::vector<Node*> possibleNodes;
		if (m_end->Front && m_end->Front != m_start && !m_end->Front->GhostNode) possibleNodes.push_back(m_end->Front);
		if (m_end->Back  && m_end->Back  != m_start && !m_end->Back->GhostNode)  possibleNodes.push_back(m_end->Back);
		if (m_end->Left  && m_end->Left  != m_start && !m_end->Left->GhostNode)  possibleNodes.push_back(m_end->Left);
		if (m_end->Right && m_end->Right != m_start && !m_end->Right->GhostNode) possibleNodes.push_back(m_end->Right);

		m_start = m_end;

		UINT n = (UINT)possibleNodes.size();
		if (n == 1)
		{
			m_end = possibleNodes[0];
		}
		else if (n > 1)
		{
			if (m_state == Eatable)
			{
				// Random AI behaviour
				m_end = possibleNodes[rand() % n];
			}
			else
			{
				// Smart AI behaviour
				Node* pathNode = Pathfinding::findPath(D3DXVECTOR3(10.f, 0.f, 10.f), m_start).back();

				bool found = false;
				for (std::vector<Node*>::iterator it = possibleNodes.begin(); it != possibleNodes.end(); it++)
				{
					if (pathNode == (*it))
					{
						m_end = pathNode;
						found = true;
						break;
					}
				}
				if (!found)
					// Random AI behaviour
					m_end = possibleNodes[rand() % n];
			}
		}
	}
}