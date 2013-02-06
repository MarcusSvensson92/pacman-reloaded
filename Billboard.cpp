#include "stdafx.h"
#include "Billboard.h"

Billboard::Billboard(const D3DXVECTOR2& size, const float alphaValue)
{
	m_size		 = size;
	m_alphaValue = alphaValue;
}

Billboard::~Billboard(void) { }

void Billboard::Draw(ID3D11DeviceContext* deviceContext, Camera camera)
{
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	D3DXMatrixTranslation(&translation, mPosition.x, mPosition.y, mPosition.z);
	world = translation;

	mShader->SetMatrix("gWorld", world);
	mShader->SetMatrix("gViewProj", camera.ViewProj());
	mShader->SetFloat3("gCameraPositionW", camera.GetPosition());
	mShader->SetFloat("gAlphaValue", m_alphaValue);
	mShader->SetResource("gTexture", mTexture);

	mVBuffer->Apply();
	mShader->Apply(0);

	deviceContext->Draw(1, 0);
}

void Billboard::InitBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	struct BillboardVertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 size;
	};

	BillboardVertex vertex;
	vertex.position = D3DXVECTOR3(0.f, 0.f, 0.f);
	vertex.size		= m_size;

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