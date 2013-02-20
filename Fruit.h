#ifndef FRUIT_H
#define FRUIT_H

#include "Candy.h"

class Fruit : public Candy
{
public:
	Fruit(void);
	~Fruit(void);
	void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader, LPCSTR texture, D3DXVECTOR3 pos, D3DXVECTOR3 scale, int points);
	void Update(const float dt);
	bool Expired() {return m_expired;}
private:
	int m_duration;
	bool m_expired;
};
#endif