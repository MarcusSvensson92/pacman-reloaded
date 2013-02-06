#include "stdafx.h"
#include "Candy.h"

Candy::Candy(void)
{
	m_objectSize = D3DXVECTOR2(4.f, 4.f);
	m_alphaValue = 1.f;

	eaten = false;
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