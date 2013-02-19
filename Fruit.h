#ifndef FRUIT_H
#define FRUIT_H

#include "Candy.h"

class Fruit : public Candy
{
public:
	Fruit(void);
	~Fruit(void);
	void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader, LPCSTR texture, D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	void Update(const float dt);
	bool Expired() {return m_expired;}
private:
	int m_duration;
	bool m_expired;
	/*
	2 fruits per stage, spawn at the same place(under the ghost cage)
	The first fruit spawns after eating 70 candy, 2nd fruit after 170 candy
	Stage/Name/Points
	1: cherry = 100 
	2: strawberry = 200
	3,4: orange = 500
	5,6: apple = 700
	7,8: grape = 1000
	9,10: ice compot = 2000
	11,12: stewed fruit = 3000
	13: key = 5000
	*/
};
#endif