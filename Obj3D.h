#pragma once
#include "stdafx.h"
#include "Shader.h"
#include "Camera.h"
#include "Buffer.h"

class Obj3D
{
	struct Vertex
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 normal;
		D3DXVECTOR2 Tex;

		D3DXVECTOR4 diff;
		D3DXVECTOR4 spec;
	};

public:
	Obj3D(void);
	Obj3D(ID3D11Device* device, ID3D11DeviceContext* deviceContext, D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	~Obj3D(void);

	void Update(ID3D11DeviceContext* deviceContext, D3DXMATRIX view);

	void Draw(ID3D11DeviceContext* deviceContext, Camera camera);

private:

	void InitBuffers( ID3D11Device* device, ID3D11DeviceContext* deviceContext );

	void LoadModel(const std::string& filename);

	void InitGFX(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	std::string 
		mModelPath,
		mShaderPath,
		mTexturePath;

	ID3D11ShaderResourceView* mTexture;

	D3DXVECTOR3 mPosition;
	std::vector<Vertex> mMesh;


	D3DXMATRIX mTexTransform;

	D3DXVECTOR3 mScale;
	D3DXVECTOR3 mWorldPos;
	D3DXVECTOR3 mRotation;

	Shader* mShader;
	Buffer* mVBuffer;

	std::vector<std::string> mModelInfo;
};

