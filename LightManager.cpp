#include "stdafx.h"
#include "LightManager.h"


LightManager::LightManager(void)
{
}


LightManager::~LightManager(void)
{
}

void LightManager::AddLight( D3DXVECTOR3* position, LightType type )
{
	Light temp;
	temp.position = position;
	temp.lightType = type;

	Lights.push_back(temp);
}

std::vector<PointLight> LightManager::SetCandyLights()
{
	std::vector<PointLight> tempList;
	PointLight standardLight;
	standardLight.Ambient =		D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f);
	standardLight.Diffuse =		D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f);
	standardLight.Specular =	D3DXVECTOR4(0.001f, 0.001f, 0.001f, 1.0f);
	standardLight.Att =			D3DXVECTOR3(0.0f,0.3f, 0.0f);
	standardLight.Range =		10;

	for(int i = 0; i < Lights.size(); i++)
	{

		standardLight.Position = *Lights[i].position;

		switch (Lights[i].lightType)
		{
		case CANDYLIGHT:

			tempList.push_back(standardLight);
			break;

		case FRUITLIGHT:

			tempList.push_back(standardLight);
			break;

		case SUPERCANDYLIGHT:

			tempList.push_back(standardLight);
			break;
		}
	}

	return tempList;
}

std::vector<PointLight> LightManager::SetMovingLights()
{
	std::vector<PointLight> tempList;
	PointLight standardLight;
	standardLight.Specular =	D3DXVECTOR4(0.001f, 0.001f, 0.001f, 1.0f);
	standardLight.Att =			D3DXVECTOR3(0.0f,0.3f, 0.0f);
	standardLight.Range =		100;

	for(int i = 0; i < Lights.size(); i++)
	{

		standardLight.Position = *Lights[i].position;

		switch (Lights[i].lightType)
		{

		case GHOSTLIGHT_RED:

			standardLight.Ambient =		D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
			standardLight.Diffuse =		D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
			tempList.push_back(standardLight);
			break;

		case GHOSTLIGHT_ORANGE:

			standardLight.Ambient =		D3DXVECTOR4(1.0f, 0.5f, 0.0f, 1.0f);
			standardLight.Diffuse =		D3DXVECTOR4(1.0f, 0.5f, 0.0f, 1.0f);
			tempList.push_back(standardLight);
			break;

		case GHOSTLIGHT_PINK:

			standardLight.Ambient =		D3DXVECTOR4(1.0f, 0.5f, 0.5f, 1.0f);
			standardLight.Diffuse =		D3DXVECTOR4(1.0f, 0.5f, 0.5f, 1.0f);
			tempList.push_back(standardLight);
			break;

		case GHOSTLIGHT_TEAL:

			standardLight.Ambient =		D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f);
			standardLight.Diffuse =		D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f);
			tempList.push_back(standardLight);
			break;

		case PACMANLIGHT:

			standardLight.Ambient =		D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
			standardLight.Diffuse =		D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
			tempList.push_back(standardLight);
			break;

		}
	}

	return tempList;
}