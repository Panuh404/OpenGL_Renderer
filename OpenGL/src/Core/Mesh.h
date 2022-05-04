#pragma once
#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Shader.h"

#include <vector>

#define MAX_BONE_INFLUENCE 4

struct m_Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;

	int m_BoneIDs[MAX_BONE_INFLUENCE];
	float m_Weights[MAX_BONE_INFLUENCE];
};

struct m_Texture
{
	uint32_t m_RendererID;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	// Mesh Data
	std::vector<m_Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<m_Texture> textures;
	uint32_t VAO;

	Mesh(std::vector<m_Vertex> vertices, std::vector<uint32_t> indices, std::vector<m_Texture> textures);

	void Draw(Shader& shader);

private:
	uint32_t VBO, IBO;
	void setupMesh();
};