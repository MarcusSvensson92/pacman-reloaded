#include "stdafx.h"
#include "GUIString.h"


GUIString::GUIString(D3DXVECTOR2 pos, float textSize)
{
	mHeight = textSize;
	mWidth = textSize;
	mPosition = pos;
}

GUIString::GUIString()
{

}


GUIString::~GUIString(void)
{
}

void GUIString::AddCharacter(ID3D11DeviceContext* deviceContext, ID3D11Device* device )
{
	GUIchar temp;
	temp.Init(mPosition + D3DXVECTOR2(mWidth*mChars.size(), 0), mHeight, mWidth, deviceContext, device);
	mChars.push_back(temp);
}

void GUIString::Reset()
{
	mChars.clear();
}

void GUIString::Update( std::string text, ID3D11DeviceContext* deviceContext, ID3D11Device* device )
{
	if(text.length() > mChars.size())
	{
		int dif = text.length() - mChars.size();
		for (int i = 0; i < dif; i++)
		{
			AddCharacter(deviceContext, device);
		}
	}

	for(int i = 0; i < text.length(); i++)
	{
		char temp = text[i];
		mChars[i].Update(atoi(&temp));
	}
}

void GUIString::Draw( ID3D11DeviceContext* deviceContext, Shader* shader, int screenWidth, int screenHeight )
{
	for(int i = 0; i < mChars.size(); i++)
	{
		mChars[i].Draw(deviceContext, shader, screenWidth, screenHeight);
	}
}