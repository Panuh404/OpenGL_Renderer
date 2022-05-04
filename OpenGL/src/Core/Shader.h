#pragma once

#include <glm.hpp>

#include <string>
#include <unordered_map>


enum shader_type
{
	VERTEX_SHADER,
	FRAGMENT_SHADER
};

class Shader
{
private:
	std::string m_VertexPath;
	std::string m_FragmentPath;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	uint32_t m_RendererID;

	Shader(const std::string& VertexPath, const std::string& FragmentPath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Uniforms utility
	void SetUniformBool		(const std::string &name, bool value) const;
	void SetUniformInt		(const std::string& name, int value) const;
	void SetUniformFloat	(const std::string& name, float value) const;
	void SetUniformFloat2	(const std::string& name, const glm::vec2& values) const;
	void SetUniformFloat3	(const std::string& name, const glm::vec3& values) const;
	void SetUniformFloat4	(const std::string& name, const glm::vec4& values) const;
	void SetUniformMat3		(const std::string& name, const glm::mat3& matrix) const;
	void SetUniformMat4		(const std::string& name, const glm::mat4& matrix) const;


private:
	uint32_t CreateShader(const std::string& VertexShaderSrc, const std::string& FragmentShaderSrc);
	uint32_t CompileShader(const std::string& filepath, shader_type type);
	bool CheckShader(uint32_t shader, bool program = false);

	int GetUniformLocation(const std::string& name) const;
};