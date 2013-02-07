#ifndef GHOST_H
#define GHOST_H

#include "Billboard.h"
#include "GhostAI.h"

class Ghost : public Billboard
{
public:
	Ghost(GhostAI* ai);
	~Ghost(void);

	void SetSpawnNode(Node* spawn);

	bool IsRoaming(void) const { return m_ai->GetState() == Roaming; }
	bool IsEatable(void) const { return m_ai->GetState() == Eatable; }
	bool IsDead(void)	 const { return m_ai->GetState() == Dead;    }

	void MakeEatable(void);
	void Kill(void);

	void Update(const float dt);
protected:
	void InitGFX(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
private:

	void UpdateVelocity(const float dt);
	void UpdateTexture(const float dt);
	void UpdateAlphaValue(void);

	GhostAI* m_ai;

	ID3D11ShaderResourceView* m_roamingTexture;
	ID3D11ShaderResourceView* m_eatableTexture1;
	ID3D11ShaderResourceView* m_eatableTexture2;

	float m_elapsedTime;
};

#endif