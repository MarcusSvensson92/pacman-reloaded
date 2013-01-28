#include "stdafx.h"
#include "Map.h"


Map::Map(void)
{
}

Map::~Map(void)
{
}

std::vector<MapOutput> Map::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, LPCSTR map, int width, int height)
{
	std::vector<MapOutput> output;

	// Create Color Map
	std::vector<int> ColorMap;
	ColorMap = CreateColorMap(map, width, height);

	// Create Mesh
	mMesh = CreateMesh(ColorMap, width, height);

	Obj3D::Init(device,deviceContext,D3DXVECTOR3(0,0,0),D3DXVECTOR3(0,0,0));
	return output;
}

std::vector<int> Map::CreateColorMap(LPCSTR Filename, int width, int height)
{
	std::vector<int> output;
	std::vector<unsigned char> c_map(width * height);
	std::ifstream inFile;
	inFile.open(Filename, std::ios_base::binary);
	if (inFile)
	{
		inFile.read((char*)&c_map[0], (std::streamsize)c_map.size());
		inFile.close();
	}
	for(int i = 0; i < (width * height); ++i)
	{
		output.push_back(c_map[i]);
	}
	return output;
}
std::vector<Vertex> Map::CreateMesh(std::vector<int> ColorMap, int width, int height)
{
	std::vector<Vertex> output;
	int z = 0, x = 0;
	for (int i = 0; i < ColorMap.size(); i++) 
	{
		//
		if ( ColorMap[i] != 0 )
		{
			//Create floor
			output.push_back(Vertex(D3DXVECTOR3(x-2.5f, -2.5f, z-2.5f), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(0,0), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			output.push_back(Vertex(D3DXVECTOR3(x-2.5f, -2.5f, z+2.5f), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(1,0), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			output.push_back(Vertex(D3DXVECTOR3(x+2.5f, -2.5f, z-2.5f), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(0,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			output.push_back(Vertex(D3DXVECTOR3(x+2.5f, -2.5f, z-2.5f), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(0,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			output.push_back(Vertex(D3DXVECTOR3(x-2.5f, -2.5f, z+2.5f), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(1,0), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			output.push_back(Vertex(D3DXVECTOR3(x+2.5f, -2.5f, z+2.5f), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(1,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			//Create ceiling
			output.push_back(Vertex(D3DXVECTOR3(x+2.5f, +2.5f, z-2.5f), D3DXVECTOR3(0, -1, 0), D3DXVECTOR2(0,0), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			output.push_back(Vertex(D3DXVECTOR3(x+2.5f, +2.5f, z+2.5f), D3DXVECTOR3(0, -1, 0), D3DXVECTOR2(1,0), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			output.push_back(Vertex(D3DXVECTOR3(x-2.5f, +2.5f, z-2.5f), D3DXVECTOR3(0, -1, 0), D3DXVECTOR2(0,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			output.push_back(Vertex(D3DXVECTOR3(x-2.5f, +2.5f, z-2.5f), D3DXVECTOR3(0, -1, 0), D3DXVECTOR2(0,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			output.push_back(Vertex(D3DXVECTOR3(x+2.5f, +2.5f, z+2.5f), D3DXVECTOR3(0, -1, 0), D3DXVECTOR2(1,0), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			output.push_back(Vertex(D3DXVECTOR3(x-2.5f, +2.5f, z+2.5f), D3DXVECTOR3(0, -1, 0), D3DXVECTOR2(1,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
		}

		// Keeping track of position
		z++;
		if (z > width)
		{
			z = 0;
			x++;
		}
	}
	return output;
}
std::vector<MapOutput> Map::CreateOutput(std::vector<int> ColorMap, int width, int height)
{
	std::vector<MapOutput> output;
	for (int i = 0; i < ColorMap.size(); i++) 
	{
		//output.push_back();

	}
	return output;
}