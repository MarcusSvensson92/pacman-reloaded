#include "stdafx.h"
#include "Candy.h"


Candy::Candy(void)
{
}

Candy::Candy(ID3D11Device* device, ID3D11DeviceContext* deviceContext, D3DXVECTOR3 pos, D3DXVECTOR3 scale) : Obj3D(device, deviceContext, pos, scale)
{
}

Candy::~Candy(void)
{
}

void Candy::Draw(ID3D11DeviceContext* g_DeviceContext,Camera camera) 
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

	g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mVBuffer->Apply();

	mShader->SetMatrix("gTextureTransform", mTexTransform);

	mShader->SetResource("mTexture", mTexture);
	mShader->Apply(0);
	g_DeviceContext->Draw(mMesh.size(),0);

}