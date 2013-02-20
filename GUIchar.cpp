#include "stdafx.h"
#include "GUIchar.h"


GUIchar::GUIchar()
{
	mFrame = D3DXVECTOR2(0,0);
}


GUIchar::~GUIchar(void)
{
}

void GUIchar::Draw(ID3D11DeviceContext* deviceContext, Shader* shader,  int screenWidth, int screenHeight)
{
	shader->SetFloat2("frame", mFrame);
	GUIElement::Draw(deviceContext, shader, screenWidth, screenHeight);
	shader->SetFloat2("frame", D3DXVECTOR2(0,0));
}


void GUIchar::Update(int frame)
{
	mFrame.x = frame/10.f;
}

void GUIchar::InitGeometry( D3DXVECTOR2 pos, float height, float width )
{
	Vertex2D temp;
	temp.pos = D3DXVECTOR3(0, 0, 1);
	temp.texc = D3DXVECTOR2(0, 0);
	mVertices.push_back(temp);
	temp.pos = D3DXVECTOR3(0, -height, 1);
	temp.texc = D3DXVECTOR2(0, 1);
	mVertices.push_back(temp);
	temp.pos = D3DXVECTOR3(width, -height, 1);
	temp.texc = D3DXVECTOR2(0.1f,1);
	mVertices.push_back(temp);
	temp.pos = D3DXVECTOR3(0, 0, 1);
	temp.texc = D3DXVECTOR2(0, 0);
	mVertices.push_back(temp);
	temp.pos = D3DXVECTOR3(width, 0, 1);
	temp.texc = D3DXVECTOR2(0.1f,0);
	mVertices.push_back(temp);
	temp.pos = D3DXVECTOR3(width, -height, 1);
	temp.texc = D3DXVECTOR2(0.1f,1);
	mVertices.push_back(temp);
}