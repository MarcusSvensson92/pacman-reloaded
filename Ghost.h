#ifndef GHOST_H
#define GHOST_H

#include "Obj3D.h"
#include "Node.h"

class Ghost : public Obj3D
{
public:
	Ghost(void);
	~Ghost(void);

	void SetStartNode(Node* node) { m_start = node; m_end = NULL; ComputeNewNodes(); }

	bool IsEatable(void) const { return m_eatable; }
	void ActivateEatable(void) { m_eatable = true; m_elapsedTime = 0.f; std::swap(m_start, m_end); }

	void Update(const float dt);
	void Draw(ID3D11DeviceContext* deviceContext, Camera camera);
protected:
	void InitBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void InitGFX(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
private:
	void UpdateVelocity(const float dt);
	void UpdateTexture(void);

	// AI Stuff
	bool IsEndNodePassed(void);
	void ComputeNewNodes(void);

	Node* m_start;
	Node* m_end;

	ID3D11ShaderResourceView* m_eatableTexture1;
	ID3D11ShaderResourceView* m_eatableTexture2;

	bool m_eatable;

	float m_elapsedTime;
};

#endif