#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "Obj3D.h"

class Billboard : public Obj3D
{
public:
	Billboard(const D3DXVECTOR2& size, const float alphaValue);
	virtual ~Billboard(void);

	virtual void Draw(ID3D11DeviceContext* deviceContext, Camera camera);
protected:
	void InitBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	D3DXVECTOR2 m_size;
	float		m_alphaValue;
};

#endif