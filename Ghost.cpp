#include "stdafx.h"
#include "Ghost.h"

Ghost::Ghost(void)
{
}

Ghost::~Ghost(void)
{
}

void Ghost::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, LPCSTR texture, D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	mShaderPath = "Content/Shaders/Billboard.fx";

	Obj3D::Init(device, deviceContext, texture, pos, scale);
}

void Ghost::InitBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	struct BillboardVertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 size;
	};

	BillboardVertex vertex;
	vertex.position = mWorldPos;
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

void Ghost::InitGFX(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION_W", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE_W",	    0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if(FAILED(mShader->Init(device, deviceContext, mShaderPath, inputDesc, 2)))
	{
		MessageBox(0, "Failed to initialize shader in Obj3D.cpp", "Fail!", 0);
	}

	if(FAILED(D3DX11CreateShaderResourceViewFromFile(device, mTexturePath.c_str(), 0, 0, &mTexture, 0 )))
	{
		std::string failed = mTexturePath + " Texture Failed";
		MessageBox(0, failed.c_str(), "Fail!", 0);
	}
}

void Ghost::Update(ID3D11DeviceContext* deviceContext, D3DXMATRIX view)
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