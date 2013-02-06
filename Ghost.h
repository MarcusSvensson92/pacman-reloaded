#ifndef GHOST_H
#define GHOST_H

#include "Billboard.h"
#include "Node.h"

class Ghost : public Billboard
{
public:
	Ghost(void);
	~Ghost(void);

	void SetSpawnNode(Node* node);

	bool IsEatable(void) const { return m_state == Eatable; }
	bool IsDead(void)	 const { return m_state == Dead;    }

	void ActivateEatable(void);
	void Kill(void);

	void Update(const float dt);
protected:
	void InitGFX(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
private:
	enum State
	{
		Roaming,
		Eatable,
		Dead
	} m_state;

	void UpdateVelocity(const float dt);
	void UpdateTexture(const float dt);

	// AI Stuff
	bool IsEndNodePassed(void);
	void ComputeNewNodes(void);

	D3DXVECTOR3 GetCurrentDirection(void) const;

	Node* m_spawn;
	Node* m_start;
	Node* m_end;

	std::vector<Node*> m_currentPath;

	ID3D11ShaderResourceView* m_roamingTexture;
	ID3D11ShaderResourceView* m_eatableTexture1;
	ID3D11ShaderResourceView* m_eatableTexture2;

	float m_elapsedTime;
};

#endif