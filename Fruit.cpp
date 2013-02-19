#include "stdafx.h"
#include "Fruit.h"


Fruit::Fruit(void)
{
	m_expired = false;
	 m_duration = 800;
	eaten = false;
	m_points = 100;
}


Fruit::~Fruit(void)
{
}

void Fruit::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader, LPCSTR texture, D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	Obj3D::Init(device,deviceContext,shader,texture,pos,scale);
	supercandy = false;
}

void Fruit::Update(const float dt)
{
	m_duration-=dt;
	if(m_duration <= 0)
	m_expired = true;
}