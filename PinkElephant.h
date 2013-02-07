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

	float m_animationPositionXZ;
	float m_animationPositionY;

	float m_kXZ;
	float m_kY;

	bool m_active;
};

#endif