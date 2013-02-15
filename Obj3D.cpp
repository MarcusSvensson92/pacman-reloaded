#include "stdafx.h"
#include "Obj3D.h"

Obj3D::Obj3D(void)
{
}

Obj3D::~Obj3D(void)
{
	delete mVBuffer;
}

void Obj3D::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader, LPCSTR texture, D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	mScale = scale;
	mPosition = pos;
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
}

void Obj3D::InitGFX(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	if(FAILED(D3DX11CreateShaderResourceViewFromFile(device, mTexturePath.c_str(), 0, 0, &mTexture, 0 )))
	{
		std::string failed = mTexturePath + " Texture Failed";
		MessageBox(0, failed.c_str(), "Fail!", 0);
	}
}

void Obj3D::InitBuffers( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
{
	BUFFER_INIT_DESC vbd;
	vbd.ElementSize = sizeof(Vertex);
	vbd.InitData = &mMesh[0];
	vbd.NumElements = mMesh.size();
	vbd.Type = VERTEX_BUFFER;
	vbd.Usage = BUFFER_CPU_WRITE;

	mVBuffer = new Buffer();
	if(FAILED(mVBuffer->Init(device, deviceContext, vbd)))
	{
		MessageBox(0, "Failed to initialize vertex buffer in Obj3D.cpp", "Fail!", 0);
	}
}

void Obj3D::Draw(ID3D11DeviceContext* m_DeviceContext,Camera camera)
{

	D3DXMatrixScaling(&scaling,mScale.x,mScale.y,mScale.z);

 	D3DXMatrixTranslation(&translation,mPosition.x,mPosition.y,mPosition.z);

	world = scaling*rotation*translation;

	wvp = world * camera.View() * camera.Proj();


	D3DXMatrixInverse(&worldInv, 0, &world);
	D3DXMatrixTranspose(&worldInvTranspose, &worldInv);

	mShader->SetMatrix("gWVP", wvp);
	mShader->SetMatrix("gWorld", world);
	mShader->SetMatrix("gWorldInvTranspose", worldInvTranspose);
	mShader->SetFloat3("gEyePos", camera.GetPosition());
	//mShader->SetRawData("gLight",&light,sizeof(PointLight));

	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mVBuffer->Apply();

	mShader->SetMatrix("gTextureTransform", mTexTransform);

	mShader->SetResource("mTexture", mTexture);
	
	mShader->Apply(0);
	m_DeviceContext->Draw(mMesh.size(),0);
	
}

D3DXVECTOR3* Obj3D::GetPositionPtr()
{
	return &mPosition;
}
