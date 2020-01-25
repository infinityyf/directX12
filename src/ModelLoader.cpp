#include "ModelLoader.h"



void Model::LoadModel(string modelPath) {
	Importer modelImporter;
	const aiScene* scene = modelImporter.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);//对导入的数据进行额外的一些计算

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "ERROR:" << modelImporter.GetErrorString() << endl;
		return;
	}

	ProcessNode(scene->mRootNode, scene);

}

//递归的处理处理场景节点
void Model::ProcessNode(aiNode* node,const aiScene* scene)
{
	//当前节点的mesh
	for (UINT i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		Meshes.push_back(ProcessMesh(mesh, scene));
	}

	//处理子节点的mesh
	for (UINT i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i], scene);
	}


}

Mesh Model::ProcessMesh(aiMesh* mesh,const aiScene* scene)
{
	vector<Vertex> vertexs;
	vector<UINT> indices;
	vector<Texture> textures;
	//处理顶点
	for (UINT i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		//生成顶点
		XMFLOAT3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;

		//assimp 最多允许一个顶点使用8个纹理坐标
		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			XMFLOAT2 uv;
			uv.x = mesh->mTextureCoords[0][i].x;
			uv.y = mesh->mTextureCoords[0][i].y;
			vertex.texcoord = uv;
		}
		else
			vertex.texcoord = XMFLOAT2(0.0f, 0.0f);

		vertexs.push_back(vertex);
	}
	//处理索引
	for (UINT i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (UINT j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	//处理材质
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture> diffuseMaps = this->loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<Texture> specularMaps = this->loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	return Mesh(vertexs, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;

	return textures;
}

Mesh::Mesh(vector<Vertex> vertexs, vector<UINT> indices, vector<Texture> textures)
{
	this->vertexs = vertexs;
	this->indices = indices;
	this->textures = textures;
}
