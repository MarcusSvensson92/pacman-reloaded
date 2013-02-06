#include "stdafx.h"
#include "Candy.h"

Candy::Candy(void)
	: Billboard(D3DXVECTOR2(4.f, 4.f), 1.f)
{
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