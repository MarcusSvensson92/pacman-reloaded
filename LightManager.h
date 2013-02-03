#pragma once

enum LightType
{
	CANDYLIGHT,
	SUPERCANDYLIGHT,
	FRUITLIGHT,
	GHOSTLIGHT_RED,
	GHOSTLIGHT_ORANGE,
	GHOSTLIGHT_PINK,
	GHOSTLIGHT_TEAL,
	PACMANLIGHT,
};

struct Light
{
	D3DXVECTOR3* position;
	LightType lightType;
};

struct PointLight
{
	PointLight() { ZeroMemory(this, sizeof(this));}

	D3DXVECTOR4 Ambient;
	D3DXVECTOR4 Diffuse;
	D3DXVECTOR4 Specular;

	D3DXVECTOR3 Position;
	float Range;

	D3DXVECTOR3 Att;
	float Pad;
};

class LightManager
{
public:
	LightManager(void);
	~LightManager(void);
	void Init();
	void AddLight(D3DXVECTOR3* position, LightType type);
	std::vector<PointLight> SetCandyLights();
	std::vector<PointLight> SetMovingLights();
private:
	std::vector<Light> Lights;
};

