#ifndef GHOST_H
#define GHOST_H

#include "Obj3D.h"

class Ghost : public Obj3D
{
public:
	Ghost(void);
	~Ghost(void);

	void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, LPCSTR texture, D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	void Update(ID3D11DeviceContext* deviceContext, D3DXMATRIX view);
	void Draw(ID3D11DeviceContext* deviceContext, Camera camera);
protected:
	void InitBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void InitGFX(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
};

#endif