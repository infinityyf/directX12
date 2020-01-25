#pragma once

#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>//用于对读取的模型文件进行后处理

#include "stdafx.h"

using namespace Assimp;
using namespace std;
using namespace DirectX;	//使用数学库

struct Vertex {
	XMFLOAT3 position;	//位置
	XMFLOAT3 normal;	//法线
	XMFLOAT2 texcoord;	//贴图坐标
};

struct Texture {
	UINT id;
	wstring type;
};

struct Mesh {
public:
	vector<Vertex> vertexs;
	vector<UINT> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vertexs,vector<UINT> indices,vector<Texture> textures);
};


class Model {
public:
	Model(string modelPath) {
		this->LoadModel(modelPath);
	}
private:
	void LoadModel(string modelPath);
	void ProcessNode(aiNode* node,const aiScene* scene);	//处理节点
	Mesh ProcessMesh(aiMesh* mesh,const aiScene* scene);	//处理mesh
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

	vector<Mesh> Meshes;
};


#endif // !MODEL_LOADER_H
