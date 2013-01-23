#include "Object3D.h"
#include <fstream>
#include <string>
#include <sstream>

Object3D::Object3D(){}
Object3D::~Object3D(void){}

Object3D::Object3D(ID3D11Device* device, float scale,D3DXVECTOR3 texTrans,D3DXVECTOR3 worldPos, std::string modelPath)
{
	mScale = scale;
	mWorldPos = worldPos;

	D3DXMatrixScaling(&texTransform, texTrans.x,texTrans.y,texTrans.z);

	mModelPath = modelPath;

	ReadModelInfo(mModelPath);

	InitTexture(device,mTexturePaths);

	InitBuffers(device);
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) 
{
	std::stringstream ss(s);
	std::string item;
	while(std::getline(ss, item, delim)) 
	{
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) 
{
	std::vector<std::string> elems;
	return split(s, delim, elems);
}

struct Face
{
	D3DXVECTOR3 positions;
	std::string material;
	std::string		smoothGroup;
	std::string		group;

	Face();

	Face(D3DXVECTOR3 pos,std::string mtl,std::string s,std::string g)
	{
		positions = pos;
		material = mtl;
		smoothGroup = s;
		group = g;
	}

};

void Object3D::ReadModelInfo(const std::string& filename, MeshData &mesh)
{
	std::ifstream fin;
	std::string prefix;

	std::vector<D3DXVECTOR3>	positions;
	std::vector<D3DXVECTOR2>	texCoords;
	std::vector<D3DXVECTOR3>	normals;
	std::vector<Face>			faces;

	std::string		material = "default";
	std::string		smoothGroup = "off";
	std::string		group = "default";
	std::string		mtllib ="default.mtl";

	UINT faceCount = 0;

	//kolla hur många faces det finns
	fin.open(filename);
	while(!fin.eof())
	{
		fin >> prefix;

		if(prefix == "f")
			faceCount++;
	}
	fin.close();

	faces.resize(faceCount);
	//faceCount återanvänds när man ger faces värden
	faceCount = 0;

	//läs .obj skapa listor av v,vt,vn,f
	fin.open(filename);
	while(true)
	{
		fin >> prefix;
		if(fin.eof())
			break;

		#pragma region Prefixes
		if(prefix == "f")
		{
			std::vector<std::string> temp;
			std::string line;

			for(int i =0;i < 3;i++)
			{
				fin >> line;
				temp = split(line, '/');

				faces[i].positions = D3DXVECTOR3(
					atoi(temp[0].c_str())-1,
					atoi(temp[1].c_str())-1,
					atoi(temp[2].c_str())-1);

					faces[i].material		= material;
					faces[i].smoothGroup	= smoothGroup;
					faces[i].group			= group;

			}

			faceCount++;
		}
		else if(prefix == "v")
		{
			float x,y,z;
			fin >> x >> y >> z;
			positions.push_back(D3DXVECTOR3(x,y,z));
		}
		else if(prefix == "vt")
		{
			float x,y;
			fin >> x >> y;
			texCoords.push_back(D3DXVECTOR2(x,y));
		}
		else if(prefix == "vn")
		{
			float x,y,z;
			fin >> x >> y >> z;
			normals.push_back(D3DXVECTOR3(x,y,z));
		}
		else if(prefix == "mtllib")
		{
			fin >> mtllib;
		}
		else if(prefix == "usemtl")
		{
			fin >> material;
		}
		else if(prefix == "s")
		{
			fin >> smoothGroup;
		}
		else if(prefix == "g")
		{
			fin >> group;
		}
		#pragma endregion Prefixes
	}
	fin.close();

	Vertex vx;

	D3DXVECTOR4 diffuse,specular,ambient;
	float		specularCoeff,transparency, bumpMultiplier;
	std::string	temp, diffuseTextureMap, bumpMap;
	bool found = false;

	//ge vertex värden från listorna
	for(int i =0;i < faces.size();i++)
	{

		vx.pos		= positions[(int)faces[i].positions.x];
		vx.Tex		= texCoords[(int)faces[i].positions.y];
		vx.normal	= normals[(int)faces[i].positions.z];

		vx.pos *= mScale;
		vx.pos += mWorldPos;

		//används samma material behövs ingen koll
		if(faces[i].material != material)
		{
			found = false;
			fin.open(mtllib);

			while(!found)
			{
				fin >> temp;

				if(fin.eof())
				{
					std::string err = faces[i].material + " Material not found!";
					MessageBox(0, err.c_str(), "Model error", 0);
					found = true;
				}

				if (temp == faces[i].material)
				{
					material = faces[i].material;

					while(true)
					{
						found = true;

						fin >> prefix;

						if(fin.eof())
							break;

						#pragma region Materials
						if (prefix == "Kd")
						{
							fin >> diffuse.x >> diffuse.y >> diffuse.z;
						}
						else if (prefix == "Ka")
						{
							fin >> ambient.x >> ambient.y >> ambient.z;
						}
						else if (prefix == "Tf")
						{
							fin >> transparency;
						}
						else if (prefix == "Ks")
						{
							fin >> specular.x >> specular.y >> specular.z;
						}
						else if (prefix == "Ns")
						{
							fin >> specularCoeff;
						}
						// lägg till textur om den inte redan finns i listan över texturer
						else if (prefix == "map_Kd") 
						{
							bool add = true;

							fin >> diffuseTextureMap;

							for(int k=0;k < mTexturePaths.size();k++)
							{
								if (diffuseTextureMap == mTexturePaths[k])
								{add = false;}
							}
							if(add)
								mTexturePaths.push_back(diffuseTextureMap);

						}
						else if (prefix == "bump")
						{
							fin >> bumpMap;
						}
						else if (prefix == "-bm")
						{
							fin >> bumpMultiplier;
						}
						else if (prefix == "newmtl")
						{
							break;
						}
						#pragma endregion Materials
					}
				}
			}

			fin.close();
		}

		vx.diff = D3DXVECTOR4(diffuse.x,diffuse.y,diffuse.z,1);
		vx.spec = D3DXVECTOR4(specular.x,specular.y,specular.z,1);

		vx.textureInfo = D3DXCOLOR(0,0,0,0);


		mesh.Vertices.push_back(vx);

	}
}

void Object3D::Update(ID3D11DeviceContext* deviceContext, D3DXMATRIX view)
{
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//Vertex* verticesPtr;

	//mMesh.Vertices.clear();

	//for(int i = 0; i < mParticles.size(); i ++)
	//{
	//	mParticles[i].Update(timeSinceLastUpdate);
	//}

	//if(FAILED(deviceContext->Map(mVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	//{
	//	return;
	//}

	//verticesPtr = (Vertex*)mappedResource.pData;
	//memcpy(verticesPtr, &mMesh.Vertices[0], sizeof(Vertex)*mMesh.Vertices.size());
	//deviceContext->Unmap(mVB, 0);
}

void Object3D::InitBuffers( ID3D11Device* device )
{
	// VertexBuffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(Vertex) * mMesh.Vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &mMesh.Vertices[0];
	device->CreateBuffer(&vbd, &vinitData, &mVB);
}

void Object3D::InitTexture( ID3D11Device* device, std::vector<std::string> paths )
{
	mTextures.resize(paths.size());

	for(int i =0;i< paths.size();i++)
	{
		if(FAILED(D3DX11CreateShaderResourceViewFromFile( device, path[i].c_str(), 0, 0, &mTextures[i], 0 )))
		{
			std::string failed = path[i] + " Texture Failed";
			MessageBox(0, failed, "Fail!", 0);
		}
	}
}

void Object3D::Draw(ID3D11DeviceContext* g_DeviceContext, Shader* g_Shader)
{
	g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_DeviceContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	g_Shader->SetMatrix("textureTransform", texTransform);

	for(int i =0; i < mTextures.size(); i++)
	{
		g_Shader->SetResource("texture", mTexture[i]);
		g_Shader->Apply(0);
		g_DeviceContext->Draw(mMesh.Vertices.size(),0);

	}

}