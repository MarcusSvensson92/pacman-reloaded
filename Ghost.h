#ifndef GHOST_H
#define GHOST_H

#include "Billboard.h"
#include "GhostAI.h"

class Ghost : public Billboard
{
public:
	Ghost(GhostAI* ai);
	~Ghost(void);

	bool IsRoaming(void) const { return m_ai->GetState() == Roaming; }
	bool IsEatable(void) const { return m_ai->GetState() == Eatable; }
	bool IsDead(void)	 const { return m_ai->GetState() == Dead;    }

	void MakeEatable(const float blueTime, const float totalTime);
	void Kill(void);
	void Reset(void);

	void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader, LPCSTR texture, D3DXVECTOR3 pos, D3DXVECTOR3 scale, Node* spawn);
	void Update(const float dt);
protected:
	void InitGFX(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
private:

	void UpdateVelocity(const float dt);
	void UpdateTexture(const float dt);
	void UpdateAlphaValue(void);

	GhostAI* m_ai;

	ID3D11ShaderResourceView* m_roamingTexture;
	ID3D11ShaderResourceView* m_eatableTexture0;
	ID3D11ShaderResourceView* m_eatableTexture1;

	float m_eatableElapsedTime;
	float m_eatableBlueTime;
	float m_eatableTotalTime;
};

#endif