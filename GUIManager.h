#pragma once
#include "stdafx.h"
#include "GUIString.h"

class GUIManager
{
public:
	GUIManager(void);
	~GUIManager(void);

	void DrawScore(ID3D11DeviceContext* deviceContext, Shader* shader,  int screenWidth, int screenHeight);
	void UpdateScore(std::string text, ID3D11DeviceContext* deviceContext, ID3D11Device* device);
	void InitScore(ID3D11DeviceContext* deviceContext, ID3D11Device* device);
	void DrawLife( ID3D11DeviceContext* deviceContext, Shader* shader, int screenWidth, int screenHeight);
	void AddLife(ID3D11DeviceContext* deviceContext, ID3D11Device* device);
	void InitLife(int lives, ID3D11DeviceContext* deviceContext, ID3D11Device* device);
	void RemoveLife();
private:
	std::vector<GUIElement*> mComponents;
	GUIString mScore;
	GUIElement mScoreLabel;
	std::vector<GUIElement> mLives;
};

