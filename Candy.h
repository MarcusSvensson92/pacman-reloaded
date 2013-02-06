#ifndef CANDY_H
#define CANDY_H

#include "Billboard.h"

class Candy : public Billboard
{
public:
	Candy(void);
	~Candy(void);

	bool IsEaten(void) { return eaten; }

	void Eat();

	void Update(const float dt);
private:
	bool eaten;
};

#endif