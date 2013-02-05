#pragma once
#include "obj3d.h"
class Candy :
	public Obj3D
{
public:
	Candy(void);
	~Candy(void);

	bool IsEaten(void) { return eaten; }

	void Eat();

	void Update(const float dt);
	void Draw(ID3D11DeviceContext* deviceContext, Camera camera);
private:
	void InitBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	bool eaten;
};

