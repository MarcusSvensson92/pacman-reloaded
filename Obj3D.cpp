#include "stdafx.h"
#include "Obj3D.h"

Obj3D::Obj3D(void)
{
}

Obj3D::~Obj3D(void)
{
}

Obj3D::Obj3D(ID3D11Device* device, ID3D11DeviceContext* deviceContext, D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	mScale = scale;
	mWorldPos = pos;
	mRotation = D3DXVECTOR3(0,0,0);

	D3DXMatrixScaling(&mTexTransform, 1,1,1);

	mModelPath = "default";
	mShaderPath = "Content/Shaders/Basic.fx";
	mTexturePath = "Content/Img/white.png";

	mShader = new Shader();

	InitGFX(device,deviceContext);

	LoadModel(mModelPath);

	InitBuffers(device, deviceContext);
}

void Obj3D::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, LPCSTR texture, D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	mScale = scale;
	mWorldPos = pos;
	mRotation = D3DXVECTOR3(0,0,0);

	D3DXMatrixScaling(&mTexTransform, 1,1,1);

	mShaderPath = "Content/Shaders/Basic.fx";
	mTexturePath = texture;

	mShader = new Shader();

	InitGFX(device,deviceContext);

	InitBuffers(device, deviceContext);
}

void Obj3D::InitGFX(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
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
	// 	D3D11_MAPPED_SUBRESOURCE mappedResource;
	// 	Vertex* verticesPtr;
	// 
	// 	mMesh.clear();
	// 
	// 	D3DXVECTOR3 rightVect;
	// 	D3DXVECTOR3 tempV = D3DXVECTOR3(view._11, view._21, view._31);
	// 	D3DXVec3Normalize(&rightVect, &tempV);
	// 	rightVect *= mScale.x;
	// 
	// 	D3DXVECTOR3 upVect;
	// 	tempV = D3DXVECTOR3(view._12, view._22, view._32);
	// 	D3DXVec3Normalize(&upVect, &tempV);
	// 	upVect *= mScale.y;
	// 
	// 	Vertex temp;
	// 	D3DXVec3Cross(&temp.normal,&rightVect,&upVect);
	// 	temp.diff	= mDiffuse;
	// 	temp.spec	= mSpecular;


	// 	temp.pos = mPosition;
	// 	temp.pos -= upVect;
	// 	temp.Tex = D3DXVECTOR2(0,1);
	// 	mMesh.push_back(temp);
	// 
	// 	temp.pos =  mPosition;
	// 	temp.pos -= upVect;
	// 	temp.Tex = D3DXVECTOR2(1,1);
	// 	mMesh.push_back(temp);
	// 
	// 	temp.pos =  mPosition;
	// 	temp.pos += rightVect;
	// 	temp.Tex = D3DXVECTOR2(0,0);
	// 	mMesh.push_back(temp);
	// 
	// 	temp.pos =  mPosition;
	// 	temp.pos += rightVect;
	// 	temp.Tex = D3DXVECTOR2(0,0);
	// 	mMesh.push_back(temp);
	// 
	// 	temp.pos = mPosition;
	// 	temp.pos -= upVect;
	// 	temp.Tex = D3DXVECTOR2(1,1);
	// 	mMesh.push_back(temp);
	// 
	// 	temp.pos = mPosition;
	// 	temp.pos += upVect;
	// 	temp.Tex = D3DXVECTOR2(1,0);
	// 	mMesh.push_back(temp);
	// 	
	// 
	// 	if(FAILED(deviceContext->Map(mVBuffer->GetBufferPointer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	// 	{
	// 		return;
	// 	}
	// 
	// 	verticesPtr = (Vertex*)mappedResource.pData;
	// 	memcpy(verticesPtr, &mMesh[0], sizeof(Vertex)*mMesh.size());
	// 	deviceContext->Unmap(mVBuffer->GetBufferPointer(), 0);
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

void Obj3D::Draw(ID3D11DeviceContext* g_DeviceContext,Camera camera)
{

	//calculate WVP matrix
	D3DXMATRIX world, wvp, worldInv, worldInvTranspose,rotation,translation,scaling;
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&rotation);
	D3DXMatrixIdentity(&translation);
	D3DXMatrixIdentity(&scaling);

	D3DXMatrixScaling(&scaling,mScale.x,mScale.y,mScale.z);

// 	D3DXMatrixRotationX(&rotation, mRotation.x);
// 	D3DXMatrixRotationY(&rotation, mRotation.y);
// 	D3DXMatrixRotationZ(&rotation, mRotation.z);

// 		V.a V.e V.i x
// 		V.b V.f V.j y
// 		V.c V.g V.k z
// 		0   0   0   1

//	D3DXMATRIX view = camera.View();

// 	rotation = D3DXMATRIX(	view._11,view._21,view._31,mPosition.x,
// 							view._12,view._22,view._32,mPosition.y,
// 							view._13,view._23,view._33,mPosition.z,
// 							0,0,0,1);

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

	mVBuffer->Apply();

	mShader->SetMatrix("gTextureTransform", mTexTransform);

	mShader->SetResource("mTexture", mTexture);
	mShader->Apply(0);
	g_DeviceContext->Draw(mMesh.size(),0);
	
}

void Obj3D::LoadModel(const std::string& filename )
{
	if(filename == "default")
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
}
