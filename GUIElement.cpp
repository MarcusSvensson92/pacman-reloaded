#include "stdafx.h"
#include "GUIElement.h"


GUIElement::GUIElement(void)
{
}


GUIElement::~GUIElement(void)
{
}

void GUIElement::Draw( ID3D11DeviceContext* deviceContext, Shader* shader, int screenWidth, int screenHeight)
{
	D3DXMATRIX view, proj, vp;

	D3DXMatrixTranslation(&view, mPosition.x - screenWidth/2, -mPosition.y + screenHeight/2, 2);
	D3DXMatrixOrthoLH(&proj, screenWidth, screenHeight, 1, 3);
	vp = view * proj;

	mVBuffer->Apply();
	shader->SetMatrix("vp", vp);
	shader->SetResource("gTexture", mTexture);
	shader->Apply(0);

	deviceContext->Draw(mVertices.size(), 0);
}

void GUIElement::Update( float deltaTime )
{

}

void GUIElement::Init(D3DXVECTOR2 pos, float height, float width, std::string texturePath, ID3D11DeviceContext* deviceContext, ID3D11Device* device)
{
	mPosition = pos;
	InitGeometry(pos, height, width);
	InitBuffer(device, deviceContext);
	InitTexture(device, texturePath);
}

void GUIElement::InitGeometry( D3DXVECTOR2 pos, float height, float width )
{
	Vertex2D temp;
	temp.pos = D3DXVECTOR3(0, 0, 1);
	temp.texc = D3DXVECTOR2(0, 0);
	mVertices.push_back(temp);
	temp.pos = D3DXVECTOR3(0, -height, 1);
	temp.texc = D3DXVECTOR2(0, 1);
	mVertices.push_back(temp);
	temp.pos = D3DXVECTOR3(width, -height, 1);
	temp.texc = D3DXVECTOR2(1,1);
	mVertices.push_back(temp);
	temp.pos = D3DXVECTOR3(0, 0, 1);
	temp.texc = D3DXVECTOR2(0, 0);
	mVertices.push_back(temp);
	temp.pos = D3DXVECTOR3(width, 0, 1);
	temp.texc = D3DXVECTOR2(1,0);
	mVertices.push_back(temp);
	temp.pos = D3DXVECTOR3(width, -height, 1);
	temp.texc = D3DXVECTOR2(1,1);
	mVertices.push_back(temp);
}

void GUIElement::InitBuffer( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
{
	BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = sizeof(Vertex2D);
	bufferDesc.InitData = &mVertices[0];
	bufferDesc.NumElements = mVertices.size();
	bufferDesc.Type = VERTEX_BUFFER;
	bufferDesc.Usage = BUFFER_DEFAULT;

	mVBuffer = new Buffer();
	if(FAILED(mVBuffer->Init(device, deviceContext, bufferDesc)))
	{
		MessageBox(0, "Buffer Failed", "Fail!", 0);
	}
}

void GUIElement::InitTexture( ID3D11Device* device, std::string path)
{
	if(FAILED(D3DX11CreateShaderResourceViewFromFile(device, path.c_str(), 0, 0, &mTexture, 0)))
	{
		MessageBox(0, "Texture Failed", "Fail!", 0);
	}
}