#ifndef PINKELEPHANT_H
#define PINKELEPHANT_H

#include "Billboard.h"
#include "Player.h"

class PinkElephant : public Billboard
{
public:
	PinkElephant(Player* player);
	~PinkElephant(void);

	void Update(const float dt);
private:
	void UpdatePosition(const float dt);
	void UpdateAnimation(const float dt);

	Player* m_player;

	float m_k;
};

#endif