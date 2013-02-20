#pragma once
#include "stdafx.h"
#include "Shader.h"
#include "Buffer.h"

struct Vertex2D
{
	D3DXVECTOR3 pos;
	D3DXVECTOR2 texc;
};

class GUIElement
{
public:
	GUIElement(void);
	~GUIElement(void);
	virtual void Draw(ID3D11DeviceContext* deviceContext, Shader* shader, int screenWidth, int screenHeight);
	virtual void Update(float deltaTime);
	virtual void Init(D3DXVECTOR2 pos, float height, float width, std::string texturePath, ID3D11DeviceContext* deviceContext, ID3D11Device* device);

protected:
	D3DXVECTOR2 mPosition;
	std::vector<Vertex2D> mVertices;
	Buffer* mVBuffer;
	ID3D11ShaderResourceView* mTexture;
	virtual void InitTexture( ID3D11Device* device, std::string path);
	virtual void InitBuffer( ID3D11Device* device, ID3D11DeviceContext* deviceContext );
	virtual void InitGeometry( D3DXVECTOR2 pos, float height, float width );
};

