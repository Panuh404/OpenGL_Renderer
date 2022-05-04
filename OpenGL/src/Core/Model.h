#pragma once
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Model
{
public:
	Model(std::string const& path, bool gamma = false);
	void Draw(Shader& shader);

	std::vector<m_Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;


private:

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene *scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<m_Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	uint32_t TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
};