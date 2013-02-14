#ifndef _MAP_H_
#define _MAP_H_

#include "stdafx.h"
#include "Obj3D.h"
#include "Node.h"

enum ObjectType
{
	PACMAN,
	CANDY,
	SUPER_CANDY,
	FRUIT,
	PINK_GHOST,
	RED_GHOST,
	ORANGE_GHOST,
	TEAL_GHOST,
};
struct MapOutput
{	
	MapOutput(ObjectType type, Node* node)
	{
		Type = type;
		Node = node;
	}
	ObjectType		Type;
	Node*			Node;
};

class Map : public Obj3D
{
public: 
	Map(void);
	~Map(void);

	std::vector<MapOutput> Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader, LPCSTR map, int width, int height);

	void Update(const float dt);
private:
	int						m_elements;
	int						m_size;
	Node					*m_nodes;

	std::vector<int>		CreateColorMap(LPCSTR Filename, int width, int height);
	std::vector<Vertex>		CreateMesh(std::vector<int> ColorMap, int width, int height);
	std::vector<MapOutput>	CreateOutput(std::vector<int> ColorMap, int width, int height);
	void					CreateNodes(std::vector<int> ColorMap, int width, int height);
	void					ConnectNodes(std::vector<int> ColorMap, int width, int height);
};

#endif