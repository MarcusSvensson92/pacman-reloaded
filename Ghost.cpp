#include "stdafx.h"
#include "Ghost.h"

Ghost::Ghost(void)
{
}

Ghost::~Ghost(void)
{
}

void Ghost::Update(const float dt)
{
}

void Ghost::Draw(ID3D11DeviceContext* deviceContext, Camera camera)
{
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	mVBuffer->Apply();

	D3DXMATRIX world, viewProj;
	D3DXMatrixIdentity(&world);
	viewProj = camera.ViewProj();

	mShader->SetMatrix("gWorld", world);
	mShader->SetMatrix("gViewProj", viewProj);
	mShader->SetFloat3("gCameraPositionW", camera.GetPosition());
	mShader->SetResource("gTexture", mTexture);

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
	vertex.position = mPosition;
	vertex.size		= D3DXVECTOR2(10.f, 10.f);

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