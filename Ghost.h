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

	void Update(const float dt);
	void Draw(ID3D11DeviceContext* deviceContext, Camera camera);
protected:
	void InitBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	// AI Stuff
	void UpdateVelocity(const float dt);
	bool IsEndNodePassed(void);

	void ComputeNewNodes(void);

	Node* m_start;
	Node* m_end;
};

#endif