#include "stdafx.h"
#include "Candy.h"


Candy::Candy(void)
{
}

Candy::Candy(ID3D11Device* device, ID3D11DeviceContext* deviceContext, D3DXVECTOR3 pos, D3DXVECTOR3 scale) : Obj3D(device, deviceContext, pos, scale)
{

}

Candy::~Candy(void)
{
}
