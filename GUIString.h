#pragma once
#include "GUIchar.h"

class GUIString
{
public:
	GUIString(D3DXVECTOR2 pos, float textSize);
	GUIString();
	~GUIString(void);
	void Update(std::string text, ID3D11DeviceContext* deviceContext, ID3D11Device* device);
	void Reset();
	void Draw(ID3D11DeviceContext* deviceContext, Shader* shader, int screenWidth, int screenHeight);

private:
	std::vector<GUIchar> mChars;
	void AddCharacter(ID3D11DeviceContext* deviceContext, ID3D11Device* device);
	D3DXVECTOR2 mPosition;
	float mHeight;
	float mWidth;
};

