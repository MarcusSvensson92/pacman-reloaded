#include "stdafx.h"
#include "GUIManager.h"


GUIManager::GUIManager(void)
{
}

GUIManager::~GUIManager(void)
{
}

void GUIManager::DrawScore( ID3D11DeviceContext* deviceContext, Shader* shader, int screenWidth, int screenHeight)
{
	mScoreLabel.Draw(deviceContext,shader, screenWidth, screenHeight);
	mScore.Draw(deviceContext, shader, screenWidth, screenHeight);
}

void GUIManager::UpdateScore( std::string text, ID3D11DeviceContext* deviceContext, ID3D11Device* device )
{
	mScore.Update(text, deviceContext, device);
}

void GUIManager::InitScore(ID3D11DeviceContext* deviceContext, ID3D11Device* device)
{
	mScoreLabel.Init(D3DXVECTOR2(0,0), 31, 174, "Content/Img/score.png", deviceContext, device);
	mScore = GUIString(D3DXVECTOR2(205,0), 31);
}

void GUIManager::DrawLife( ID3D11DeviceContext* deviceContext, Shader* shader,  int screenWidth, int screenHeight )
{
	for(int i = 0; i < mLives.size(); i++)
		mLives[i].Draw(deviceContext, shader, screenWidth, screenHeight);
}

void GUIManager::InitLife(int lives, ID3D11DeviceContext* deviceContext, ID3D11Device* device)
{
	for (int i = 0; i < lives; i++)
		AddLife(deviceContext, device);
}

void GUIManager::AddLife(ID3D11DeviceContext* deviceContext, ID3D11Device* device)
{
	GUIElement temp;
	temp.Init(D3DXVECTOR2(28*mLives.size(), 41), 28, 28, "Content/Img/life.png", deviceContext, device);
	mLives.push_back(temp);
}

void GUIManager::RemoveLife()
{
	mLives.pop_back();
}