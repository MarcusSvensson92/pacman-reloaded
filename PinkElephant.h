#ifndef PINKELEPHANT_H
#define PINKELEPHANT_H

#include "Billboard.h"
#include "Player.h"

class PinkElephant : public Billboard
{
public:
	PinkElephant(Player* player);
	~PinkElephant(void);

	void Activate(void)   { m_active = true; }
	void Deactivate(void) { m_active = false; }

	void Update(const float dt);
	void Draw(ID3D11DeviceContext* deviceContext, Camera camera);
private:
	void UpdatePosition(const float dt);
	void UpdateAnimation(const float dt);

	Player* m_player;

	D3DXVECTOR3 m_animationPosition;
	D3DXVECTOR3 m_animationK;

	bool m_active;
};

#endif