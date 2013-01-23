#pragma once
#include "stdafx.h"
#include "Shader.h"

class Object3D
{
	struct Vertex
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 normal;
		D3DXVECTOR2 Tex;

		D3DXVECTOR4 diff;
		D3DXVECTOR4 spec;

		D3DXCOLOR textureInfo;
	};

	struct MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<UINT> Indices;
	};

public:
	Object3D(void);
	Object3D(ID3D11Device* device, float scale,D3DXVECTOR3 texTrans,D3DXVECTOR3 worldPos, std::string modelPath);
	~Object3D(void);

	void Update(ID3D11DeviceContext* deviceContext, D3DXMATRIX view);

	void Draw(ID3D11DeviceContext* g_DeviceContext, Shader* g_Shader);

private:
	void InitTexture( ID3D11Device* device , std::vector<std::string> paths);
	void InitBuffers( ID3D11Device* device );
	void ReadModelInfo(const std::string& filename, MeshData &mesh);

	void Indices(std::ifstream &fin, std::string ignore,UINT k );

	void Normals( std::ifstream &fin, std::string ignore, UINT k );

	void TextureCoords( std::ifstream &fin, std::string ignore, UINT k );

	void Positions( std::ifstream &fin, std::string ignore, UINT k );

	std::string mModelPath;
	std::vector<ID3D11ShaderResourceView*> mTextures;
	D3DXVECTOR3 mPosition;
	MeshData mMesh;
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	D3DXMATRIX texTransform;

	std::vector<std::string>	mTexturePaths;

	int mScale;
	D3DXVECTOR3 mWorldPos;

	std::vector<std::string> mModelInfo;
};

