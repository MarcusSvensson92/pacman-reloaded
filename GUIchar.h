#pragma once
#include "GUIElement.h"

class GUIchar : public GUIElement
{
public:
	GUIchar();
	~GUIchar(void);
	void Init(D3DXVECTOR2 pos, float height, float width, ID3D11DeviceContext* deviceContext, ID3D11Device* device);
	void Update(int frame);
	void Draw(ID3D11DeviceContext* deviceContext, Shader* shader, int screenWidth, int screenHeight);
private:

	void InitGeometry( D3DXVECTOR2 pos, float height, float width );
	D3DXVECTOR2 mFrame;
	
};

