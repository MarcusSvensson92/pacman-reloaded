#pragma once
#include "obj3d.h"
class Candy :
	public Obj3D
{
public:
	Candy(void);
	Candy(ID3D11Device* device, ID3D11DeviceContext* deviceContext, D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	~Candy(void);
	virtual void Draw(ID3D11DeviceContext* deviceContext, Camera camera);
	void Eat(){eaten=true;};

	bool eaten;
};

