#include "stdafx.h"
#include "Candy.h"

Candy::Candy(void)
{
	LoadModel();
}

Candy::~Candy(void)
{
}

void Candy::Update(const float dt)
{
}

void Candy::Draw(ID3D11DeviceContext* m_DeviceContext,Camera camera) 
{
	D3DXMatrixScaling(&scaling,mScale.x,mScale.y,mScale.z);

	// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
	float angle = atan2(camera.GetPosition().x - mPosition.x, camera.GetPosition().z - mPosition.z) * (180.0 / D3DX_PI);

	// Convert rotation into radians.
	float rot = (float)angle * 0.0174532925f;
	//Use the rotation to first rotate the world matrix accordingly, and then translate to the position of the billboard in the world.

	// Setup the rotation the billboard at the origin using the world matrix.
	D3DXMatrixRotationY(&world, rot);

	// Setup the translation matrix from the billboard model.
	D3DXMatrixTranslation(&translation, mPosition.x, mPosition.y, mPosition.z);

	// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
	D3DXMatrixMultiply(&world, &world, &translation); 

	world = scaling*world;

	wvp = world * camera.View() * camera.Proj();


	D3DXMatrixInverse(&worldInv, 0, &world);
	D3DXMatrixTranspose(&worldInvTranspose, &worldInv);

	mShader->SetMatrix("gWVP", wvp);
	mShader->SetMatrix("gWorld", world);
	mShader->SetMatrix("gWorldInvTranspose", worldInvTranspose);
	mShader->SetFloat3("gEyePosW", camera.GetPosition());
	//mShader->SetRawData("gLight",&light,sizeof(PointLight));

	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mVBuffer->Apply();

	mShader->SetMatrix("gTextureTransform", mTexTransform);

	mShader->SetResource("mTexture", mTexture);
	mShader->Apply(0);
	m_DeviceContext->Draw(mMesh.size(),0);

}

void Candy::LoadModel(void)
{
	Vertex vx;

	vx.pos = D3DXVECTOR3(-1,-1,0);
	vx.diff = D3DXVECTOR4(0.5f,0.5f,0.5f,1);
	vx.spec = D3DXVECTOR4(0.5f,0.5f,0.5f,1);
	vx.normal = D3DXVECTOR3(0,0,-1);
	vx.Tex = D3DXVECTOR2(0,0);
	mMesh.push_back(vx);

	vx.pos = D3DXVECTOR3(1,1,0);
	vx.diff = D3DXVECTOR4(0.5f,0.5f,0.5f,1);
	vx.spec = D3DXVECTOR4(0.5f,0.5f,0.5f,1);
	vx.normal = D3DXVECTOR3(0,0,-1);
	vx.Tex = D3DXVECTOR2(1,1);
	mMesh.push_back(vx);
		
	vx.pos = D3DXVECTOR3(-1,1,0);
	vx.diff = D3DXVECTOR4(0.5f,0.5f,0.5f,1);
	vx.spec = D3DXVECTOR4(0.5f,0.5f,0.5f,1);
	vx.normal = D3DXVECTOR3(0,0,-1);
	vx.Tex = D3DXVECTOR2(0,1);
	mMesh.push_back(vx);

	vx.pos = D3DXVECTOR3(-1,-1,0);
	vx.diff = D3DXVECTOR4(0.5f,0.5f,0.5f,1);
	vx.spec = D3DXVECTOR4(0.5f,0.5f,0.5f,1);
	vx.normal = D3DXVECTOR3(0,0,-1);
	vx.Tex = D3DXVECTOR2(0,0);
	mMesh.push_back(vx);

	vx.pos = D3DXVECTOR3(1,-1,0);
	vx.diff = D3DXVECTOR4(0.5f,0.5f,0.5f,1);
	vx.spec = D3DXVECTOR4(0.5f,0.5f,0.5f,1);
	vx.normal = D3DXVECTOR3(0,0,-1);
	vx.Tex = D3DXVECTOR2(1,0);
	mMesh.push_back(vx);

	vx.pos = D3DXVECTOR3(1,1,0);
	vx.diff = D3DXVECTOR4(0.5f,0.5f,0.5f,1);
	vx.spec = D3DXVECTOR4(0.5f,0.5f,0.5f,1);
	vx.normal = D3DXVECTOR3(0,0,-1);
	vx.Tex = D3DXVECTOR2(1,1);
	mMesh.push_back(vx);
}