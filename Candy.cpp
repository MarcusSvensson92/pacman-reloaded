#include "stdafx.h"
#include "Candy.h"

Candy::Candy(void)
	: Billboard(D3DXVECTOR2(4.f, 4.f), 1.f)
{
	eaten = false;
}

void Candy::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader, LPCSTR texture, D3DXVECTOR3 pos, D3DXVECTOR3 scale,bool _supercandy)
{
	Obj3D::Init(device,deviceContext,shader,texture,pos,scale);
	supercandy = _supercandy;
}

Candy::~Candy(void)
{
}

void Candy::Update(const float dt)
{
}

void Candy::Eat() 
{ 
	eaten = true;
}