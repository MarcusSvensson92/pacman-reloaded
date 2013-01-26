#include "stdafx.h"
#include "Obj3D.h"


Obj3D::Obj3D(void)
{
}


Obj3D::~Obj3D(void)
{
}

Obj3D::Obj3D(ID3D11Device* device,ID3D11DeviceContext* deviceContext,D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	mScale = scale;
	mWorldPos = pos;

	D3DXMatrixScaling(&mTexTransform, 1,1,1);

	mModelPath = "default";
	mShaderPath = "../Shaders/Obj3D.fx";

	InitGFX(device,deviceContext);

	InitBuffers(device);

	mShader = new Shader();

}

void Obj3D::InitGFX(ID3D11Device* device,ID3D11DeviceContext* deviceContext)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "DIFFUSE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SPECULAR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	if(FAILED(mShader->Init(device, deviceContext, mShaderPath, inputDesc, 5)))
	{
		MessageBox(0, "Failed to initialize shader in Obj3D.cpp", "Fail!", 0);
	}

	if(FAILED(D3DX11CreateShaderResourceViewFromFile(device, mTexturePath.c_str(), 0, 0, &mTexture, 0 )))
	{
		std::string failed = mTexturePath + " Texture Failed";
		MessageBox(0, failed.c_str(), "Fail!", 0);
	}
}

void Obj3D::Update(ID3D11DeviceContext* deviceContext, D3DXMATRIX view)
{
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//Vertex* verticesPtr;

	//mMesh.Vertices.clear();

	//for(int i = 0; i < mParticles.size(); i ++)
	//{
	//	mParticles[i].Update(timeSinceLastUpdate);
	//}

	//if(FAILED(deviceContext->Map(mVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	//{
	//	return;
	//}

	//verticesPtr = (Vertex*)mappedResource.pData;
	//memcpy(verticesPtr, &mMesh.Vertices[0], sizeof(Vertex)*mMesh.Vertices.size());
	//deviceContext->Unmap(mVB, 0);
}

void Obj3D::InitBuffers( ID3D11Device* device )
{
	// VertexBuffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(Vertex) * mMesh.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &mMesh[0];
	device->CreateBuffer(&vbd, &vinitData, &mVB);
}

void Obj3D::Draw(ID3D11DeviceContext* g_DeviceContext,Camera camera)
{

	//calculate WVP matrix
	D3DXMATRIX world, wvp, worldInv, worldInvTranspose,rotation,translation,scaling;
	D3DXMatrixIdentity(&world);

	D3DXMatrixScaling(&scaling,mScale.x,mScale.y,mScale.z);

	D3DXMatrixRotationX(&rotation, mRotation.x);
	D3DXMatrixRotationY(&rotation, mRotation.y);
	D3DXMatrixRotationZ(&rotation, mRotation.z);

	D3DXMatrixTranslation(&translation,mWorldPos.x,mWorldPos.y,mWorldPos.z);

	world = scaling*rotation*translation;

	wvp = world * camera.View() * camera.Proj();


	D3DXMatrixInverse(&worldInv, 0, &world);
	D3DXMatrixTranspose(&worldInvTranspose, &worldInv);

	mShader->SetMatrix("gWVP", wvp);
	mShader->SetMatrix("gWorld", world);
	mShader->SetMatrix("gWorldInvTranspose", worldInvTranspose);
	mShader->SetFloat3("gEyePosW", camera.GetPosition());
	//mShader->SetRawData("gLight",&light,sizeof(PointLight));

	g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_DeviceContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	mShader->SetMatrix("gTextureTransform", mTexTransform);

	mShader->SetResource("mTexture", mTexture);
	mShader->Apply(0);
	g_DeviceContext->Draw(mMesh.size(),0);
	
}