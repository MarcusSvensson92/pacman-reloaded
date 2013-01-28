#ifndef _MAP_H_
#define _MAP_H_

#include "stdafx.h"
#include "Obj3D.h"

struct Node
{
	Node* Up;
	D3DXVECTOR3 Position;
};
struct MapOutput
{	
	enum ObjectType
	{
		PACMAN,
		CANDY,
		SUPER_CANDY,
		PINK_GHOST,
		RED_GHOST,
		ORANGE_GHOST,
		TEAL_GHOST,
	};
	MapOutput(ObjectType type, Node* node)
	{
		Type = type;
		Node = node;
	}
	ObjectType Type;
	Node* Node;
};

class Map : public Obj3D
{
public: 
	Map(void);
	~Map(void);

	std::vector<MapOutput> Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, LPCSTR map, int width, int height);
	//void Update(float dt);
	//void Draw();
private:
	std::vector<int>		CreateColorMap(LPCSTR Filename, int width, int height);
	std::vector<Vertex>		CreateMesh(std::vector<int> ColorMap, int width, int height);
	std::vector<MapOutput>	CreateOutput(std::vector<int> ColorMap, int width, int height);
};

#endif