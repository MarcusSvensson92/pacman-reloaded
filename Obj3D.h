#pragma once
#include "stdafx.h"
#include "Shader.h"
#include "Camera.h"
#include "Buffer.h"

struct Vertex
{
	Vertex() {}
	Vertex(	D3DXVECTOR3 p, D3DXVECTOR3 n, D3DXVECTOR2 t, D3DXVECTOR4 d, D3DXVECTOR4 s )
	{
		pos = p;
		normal = n;
		Tex = t;
		diff = d;
		spec = s;
	}
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 Tex;

	D3DXVECTOR4 diff;
	D3DXVECTOR4 spec;
};
class Obj3D
{
public:
	Obj3D(void);
	Obj3D(ID3D11Device* device, ID3D11DeviceContext* deviceContext, D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	~Obj3D(void);

	void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, LPCSTR texture, D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	void Update(ID3D11DeviceContext* deviceContext, D3DXMATRIX view);

	virtual void Draw(ID3D11DeviceContext* deviceContext, Camera camera);
protected:
	std::vector<Vertex> mMesh;
	D3DXVECTOR3 mScale;
	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mRotation;

	Shader* mShader;
	D3DXMATRIX mTexTransform;
	ID3D11ShaderResourceView* mTexture;
	Buffer* mVBuffer;

	D3DXMATRIX world, wvp, worldInv, worldInvTranspose,rotation,translation,scaling;

private:

	void InitBuffers( ID3D11Device* device, ID3D11DeviceContext* deviceContext );

	void LoadModel(const std::string& filename);

	void InitGFX(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	std::string 
		mModelPath,
		mShaderPath,
		mTexturePath;

	std::vector<std::string> mModelInfo;
};

