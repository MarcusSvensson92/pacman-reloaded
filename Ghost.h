#ifndef GHOST_H
#define GHOST_H

#include "Obj3D.h"
#include "Node.h"

class Ghost : public Obj3D
{
public:
	Ghost(void);
	~Ghost(void);

	void SetSpawnNode(Node* node);

	bool IsEatable(void) const { return m_state == Eatable; }
	bool IsEated(void)	 const { return m_state == Eated;   }

	void ActivateEatable(void);

	void Update(const float dt);
	void Draw(ID3D11DeviceContext* deviceContext, Camera camera);
protected:
	void InitBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void InitGFX(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
private:
	enum State
	{
		Roaming,
		Eatable,
		Eated
	} m_state;

	void UpdateVelocity(const float dt);
	void UpdateTexture(void);

	// AI Stuff
	bool IsEndNodePassed(void);
	void ComputeNewNodes(void);

	Node* m_spawn;
	Node* m_start;
	Node* m_end;

	ID3D11ShaderResourceView* m_eatableTexture1;
	ID3D11ShaderResourceView* m_eatableTexture2;

	float m_elapsedTime;
};

#endif