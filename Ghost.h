#ifndef GHOST_H
#define GHOST_H

#include "Obj3D.h"

class Ghost : public Obj3D
{
public:
	Ghost(void);
	~Ghost(void);

	void Update(const float dt);
	void Draw(ID3D11DeviceContext* deviceContext, Camera camera);
protected:
	void InitBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
};

#endif