#include "stdafx.h"
#include "Map.h"


Map::Map(void)
{
}

Map::~Map(void)
{
	delete m_nodes;
}

std::vector<MapOutput> Map::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, LPCSTR map, int width, int height)
{
	std::vector<MapOutput> output;
	m_size = 10;

	// Create Color Map
	std::vector<int> ColorMap;
	ColorMap = CreateColorMap(map, width, height);

	// Create Mesh
	mMesh = CreateMesh(ColorMap, width, height);

	// Create Nodes
	m_elements = width*height;
	CreateNodes(ColorMap, width, height);
	ConnectNodes(ColorMap, width, height);

	// Create Output
	output = CreateOutput(ColorMap, width, height);

	// Init d3d
	Obj3D::Init(device,deviceContext,"Content/Img/mazetexture.png",D3DXVECTOR3(0,0,0),D3DXVECTOR3(1,1,1));
	return output;
}

std::vector<int>		Map::CreateColorMap(LPCSTR Filename, int width, int height)
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
std::vector<Vertex>		Map::CreateMesh(std::vector<int> ColorMap, int width, int height)
{
	std::vector<Vertex> output;
	float z = 0, x = 0;
	for (int i = 0; i < ColorMap.size(); i++) 
	{
		//
		if ( ColorMap[i] != 0 )
		{
			//Create floor
			output.push_back(Vertex(D3DXVECTOR3(x-(m_size*0.5), -(m_size*0.5), z-(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(0, 0.35f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			output.push_back(Vertex(D3DXVECTOR3(x-(m_size*0.5), -(m_size*0.5), z+(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(1, 0.35f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			output.push_back(Vertex(D3DXVECTOR3(x+(m_size*0.5), -(m_size*0.5), z-(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(0, 0.60f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			output.push_back(Vertex(D3DXVECTOR3(x+(m_size*0.5), -(m_size*0.5), z-(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(0, 0.60f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			output.push_back(Vertex(D3DXVECTOR3(x-(m_size*0.5), -(m_size*0.5), z+(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(1, 0.35f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			output.push_back(Vertex(D3DXVECTOR3(x+(m_size*0.5), -(m_size*0.5), z+(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(1, 0.60f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			//Create ceiling
			output.push_back(Vertex(D3DXVECTOR3(x+(m_size*0.5), +(m_size*0.5), z-(m_size*0.5)), D3DXVECTOR3(0, -1, 0), D3DXVECTOR2(0, 0.05f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			output.push_back(Vertex(D3DXVECTOR3(x+(m_size*0.5), +(m_size*0.5), z+(m_size*0.5)), D3DXVECTOR3(0, -1, 0), D3DXVECTOR2(1,	0.05f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			output.push_back(Vertex(D3DXVECTOR3(x-(m_size*0.5), +(m_size*0.5), z-(m_size*0.5)), D3DXVECTOR3(0, -1, 0), D3DXVECTOR2(0, 0.30f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			output.push_back(Vertex(D3DXVECTOR3(x-(m_size*0.5), +(m_size*0.5), z-(m_size*0.5)), D3DXVECTOR3(0, -1, 0), D3DXVECTOR2(0, 0.30f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			output.push_back(Vertex(D3DXVECTOR3(x+(m_size*0.5), +(m_size*0.5), z+(m_size*0.5)), D3DXVECTOR3(0, -1, 0), D3DXVECTOR2(1, 0.05f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			output.push_back(Vertex(D3DXVECTOR3(x-(m_size*0.5), +(m_size*0.5), z+(m_size*0.5)), D3DXVECTOR3(0, -1, 0), D3DXVECTOR2(1, 0.30f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			//Create front wall
			if ( i-width >= 0 )
			{
				if ( ColorMap[i-width] == 0 )
				{
					output.push_back(Vertex(D3DXVECTOR3(x-(m_size*0.5), +(m_size*0.5), z-(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(0, 0.70f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
					output.push_back(Vertex(D3DXVECTOR3(x-(m_size*0.5), +(m_size*0.5), z+(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(1, 0.70f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
					output.push_back(Vertex(D3DXVECTOR3(x-(m_size*0.5), -(m_size*0.5), z-(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(0, 0.95f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
					output.push_back(Vertex(D3DXVECTOR3(x-(m_size*0.5), -(m_size*0.5), z-(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(0, 0.95f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
					output.push_back(Vertex(D3DXVECTOR3(x-(m_size*0.5), +(m_size*0.5), z+(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(1, 0.70f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
					output.push_back(Vertex(D3DXVECTOR3(x-(m_size*0.5), -(m_size*0.5), z+(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(1, 0.95f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
				}
			}
			//Create back wall
			if (i+width < width*height)
			{
				if ( ColorMap[i+width] == 0 )
				{
					output.push_back(Vertex(D3DXVECTOR3(x+(m_size*0.5), +(m_size*0.5), z+(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(0, 0.70f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
					output.push_back(Vertex(D3DXVECTOR3(x+(m_size*0.5), +(m_size*0.5), z-(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(1, 0.70f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
					output.push_back(Vertex(D3DXVECTOR3(x+(m_size*0.5), -(m_size*0.5), z+(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(0, 0.95f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
					output.push_back(Vertex(D3DXVECTOR3(x+(m_size*0.5), -(m_size*0.5), z+(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(0, 0.95f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
					output.push_back(Vertex(D3DXVECTOR3(x+(m_size*0.5), +(m_size*0.5), z-(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(1, 0.70f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
					output.push_back(Vertex(D3DXVECTOR3(x+(m_size*0.5), -(m_size*0.5), z-(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(1, 0.95f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
				}
			}
			//Create left wall
			if ( ColorMap[i-1] == 0 )
			{
				output.push_back(Vertex(D3DXVECTOR3(x+(m_size*0.5), +(m_size*0.5), z-(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(0, 0.70f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
				output.push_back(Vertex(D3DXVECTOR3(x-(m_size*0.5), +(m_size*0.5), z-(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(1, 0.70f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
				output.push_back(Vertex(D3DXVECTOR3(x+(m_size*0.5), -(m_size*0.5), z-(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(0, 0.95f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
				output.push_back(Vertex(D3DXVECTOR3(x+(m_size*0.5), -(m_size*0.5), z-(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(0, 0.95f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
				output.push_back(Vertex(D3DXVECTOR3(x-(m_size*0.5), +(m_size*0.5), z-(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(1, 0.70f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
				output.push_back(Vertex(D3DXVECTOR3(x-(m_size*0.5), -(m_size*0.5), z-(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(1, 0.95f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			}
			//Create right wall
			if ( ColorMap[i+1] == 0 )
			{
				output.push_back(Vertex(D3DXVECTOR3(x-(m_size*0.5), +(m_size*0.5), z+(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(0, 0.70f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
				output.push_back(Vertex(D3DXVECTOR3(x+(m_size*0.5), +(m_size*0.5), z+(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(1, 0.70f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
				output.push_back(Vertex(D3DXVECTOR3(x-(m_size*0.5), -(m_size*0.5), z+(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(0, 0.95f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
				output.push_back(Vertex(D3DXVECTOR3(x-(m_size*0.5), -(m_size*0.5), z+(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(0, 0.95f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
				output.push_back(Vertex(D3DXVECTOR3(x+(m_size*0.5), +(m_size*0.5), z+(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(1, 0.70f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
				output.push_back(Vertex(D3DXVECTOR3(x+(m_size*0.5), -(m_size*0.5), z+(m_size*0.5)), D3DXVECTOR3(0, +1, 0), D3DXVECTOR2(1, 0.95f), D3DXVECTOR4(0.5f,0.5f,0.5f,1), D3DXVECTOR4(0.5f,0.5f,0.5f,1)));
			}
		}

		// Keeping track of position
		z += m_size;
		if (z >= width*m_size)
		{
			z = 0;
			x += m_size;
		}
	}
	return output;
}
std::vector<MapOutput>	Map::CreateOutput(std::vector<int> ColorMap, int width, int height)
{
	std::vector<MapOutput> output;
	int z = 0, x = 0;
	for (int i = 0; i < ColorMap.size(); i++) 
	{
		// Add output
		if		( ColorMap[i] == 65 )
		{
			output.push_back(MapOutput(CANDY,			&m_nodes[i]));
		}
		else if ( ColorMap[i] == 116 )
		{
			output.push_back(MapOutput(SUPER_CANDY,		&m_nodes[i]));
		}
		else if ( ColorMap[i] == 248 )
		{
			output.push_back(MapOutput(PACMAN,			&m_nodes[i]));
		}
		else if ( ColorMap[i] == 118 )
		{
			output.push_back(MapOutput(PINK_GHOST,		&m_nodes[i]));
		}
		else if ( ColorMap[i] == 119 )
		{
			output.push_back(MapOutput(RED_GHOST,		&m_nodes[i]));
		}
		else if ( ColorMap[i] == 120 )
		{
			output.push_back(MapOutput(ORANGE_GHOST,	&m_nodes[i]));
		}
		else if ( ColorMap[i] == 121 )
		{
			output.push_back(MapOutput(TEAL_GHOST,		&m_nodes[i]));
		}
	}
	return output;
}
void					Map::CreateNodes(std::vector<int> ColorMap, int width, int height)
{
	m_nodes = new Node[width*height];
	int z = 0, x = 0;
	for (int i = 0; i < ColorMap.size(); i++) 
	{
		// Add node
		if ( ColorMap[i] != 0 )
		{
			m_nodes[i] = Node(D3DXVECTOR3(x, 0, z));
		}

		// Keeping track of position
		z += m_size;
		if (z >= width*m_size)
		{
			z = 0;
			x += m_size;
		}
	}
}
void					Map::ConnectNodes(std::vector<int> ColorMap, int width, int height)
{
	int z = 0, x = 0;
	for (int i = 0; i < ColorMap.size(); i++) 
	{
		// Add node
		if ( ColorMap[i] != 0 )
		{
			// Connect back node
			if (i+width < width*height)
			{
				if ( ColorMap[i+width] != 0 && m_nodes[i].Back == NULL )
				{
					m_nodes[i].Back = &m_nodes[i+width];
					m_nodes[i+width].Front = &m_nodes[i];
				}
			}
			// Connect right node
			if ( ColorMap[i+1] != 0 && m_nodes[i].Right == NULL )
			{
				m_nodes[i].Right = &m_nodes[i+1];
				m_nodes[i+1].Left = &m_nodes[i];
			}
		}

		// Keeping track of position
		z += m_size;
		if (z >= width*m_size)
		{
			z = 0;
			x += m_size;
		}
	}
}