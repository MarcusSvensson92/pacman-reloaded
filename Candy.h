#ifndef CANDY_H
#define CANDY_H

#include "Billboard.h"

class Candy : public Billboard
{
public:
	Candy(void);
	~Candy(void);

	bool IsEaten(void) { return eaten; }
	bool IsSuperCandy() {return supercandy;}

	void Eat();
	void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader, LPCSTR texture, D3DXVECTOR3 pos, D3DXVECTOR3 scale,bool _supercandy);
	void Update(const float dt);
private:
	bool eaten;
protected:
	bool supercandy;
};

#endif