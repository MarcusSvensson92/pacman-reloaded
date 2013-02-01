#include "stdafx.h"
#include "Ghost.h"
#include <time.h>
#include <cstdlib>
#include "PathFinding.h"

const D3DXVECTOR2 g_ghostSize  = D3DXVECTOR2(7.f, 7.f);
const float		  g_ghostSpeed = 20.f;

Ghost::Ghost(void)
{
	m_start = NULL;
	m_end   = NULL;

	srand(time(NULL));
}

Ghost::~Ghost(void)
{
}

void Ghost::Update(const float dt)
{
	UpdateVelocity(dt);
}

void Ghost::Draw(ID3D11DeviceContext* deviceContext, Camera camera)
{
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	D3DXMatrixTranslation(&translation, mPosition.x, mPosition.y, mPosition.z);
	world = translation;

	mShader->SetMatrix("gWorld", world);
	mShader->SetMatrix("gViewProj", camera.ViewProj());
	mShader->SetFloat3("gCameraPositionW", camera.GetPosition());
	mShader->SetResource("gTexture", mTexture);

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

void Ghost::UpdateVelocity(const float dt)
{
	D3DXVECTOR3 direction = m_end->GetPosition() - m_start->GetPosition();
	D3DXVec3Normalize(&direction, &direction);

	mPosition += direction * g_ghostSpeed * dt;

	if (IsEndNodePassed())
	{
		ComputeNewNodes();
	}
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
	if (!m_end)
		m_end = m_start;

	std::vector<Node*> possibleNodes;
	if (m_end->Front && m_end->Front != m_start) possibleNodes.push_back(m_end->Front);
	if (m_end->Back  && m_end->Back  != m_start) possibleNodes.push_back(m_end->Back);
	if (m_end->Left  && m_end->Left  != m_start) possibleNodes.push_back(m_end->Left);
	if (m_end->Right && m_end->Right != m_start) possibleNodes.push_back(m_end->Right);

	m_start = m_end;

	UINT n = (UINT)possibleNodes.size();
	if (n == 1)
	{
		m_end = possibleNodes[0];
	}
	else
	{
		// Random AI behaviour
		// m_end = possibleNodes[rand() % n];

		// Smart AI behaviour
		m_end = Pathfinding::findPath(D3DXVECTOR3(10.f, 0.f, 10.f), m_start).back();
	}
}