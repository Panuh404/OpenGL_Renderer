#include <iostream>
#include <fstream>

#include <glad/glad.h>

#include "Shader.h"

Shader::Shader(const std::string& VertexPath, const std::string& FragmentPath) :
	m_VertexPath(VertexPath), m_FragmentPath(FragmentPath), m_RendererID(0)
{
	const std::string& VertexShaderSrc = VertexPath;
	const std::string& FragmentShaderSrc = FragmentPath;
	m_RendererID = CreateShader(VertexShaderSrc, FragmentShaderSrc);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}


uint32_t Shader::CompileShader(const std::string& filepath, shader_type type)
{
	std::ifstream ShaderStream(filepath);
	std::string shaderSource;
	std::string Line;

	while (std::getline(ShaderStream, Line))
	{
		shaderSource += "\n" + Line;
	}

	uint32_t shader;
	const char* src = shaderSource.c_str();

	if(type == VERTEX_SHADER)
	{
		shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);
		if(!CheckShader(shader))
		{
			glDeleteShader(shader);
			return 0;
		}
		return shader;
	}
	if(type == FRAGMENT_SHADER)
	{
		shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);
		if (!CheckShader(shader))
		{
			glDeleteShader(shader);
			return 0;
		}
		return shader;
	}
}

uint32_t Shader::CreateShader(const std::string& VertexShaderSrc, const std::string& FragmentShaderSrc)
{
	const uint32_t vertexShader =	CompileShader(VertexShaderSrc, VERTEX_SHADER);
	const uint32_t fragmentShader = CompileShader(FragmentShaderSrc, FRAGMENT_SHADER);
	const uint32_t shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);
	glValidateProgram(shaderProgram);

	if (!CheckShader(shaderProgram, true))
	{
		glDeleteProgram(shaderProgram);
		return 0;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

bool Shader::CheckShader(uint32_t shader, bool program)
{
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success and !program) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}
	if (!success and program) {
		glGetProgramInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}
	if (success and !program) {
		std::cout << "SHADER::COMPILATION_SUCCESSFUL" << std::endl;
		return true;
	}
	if (success and program) {
		std::cout << "PROGRAM::COMPILATION_SUCCESSFUL" << std::endl;
		return true;
	}
	return true;
}

int Shader::GetUniformLocation(const std::string& name) const
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[name];
	}
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1) {
		std::cout << "Warning: uniform " << name << " not found!" << std::endl;
	}
	m_UniformLocationCache[name] = location;
	return location;
}

void Shader::SetUniformBool(const std::string& name, bool value) const
{
	glUniform1i(GetUniformLocation(name), (int)value);
}

void Shader::SetUniformInt(const std::string& name, int value) const
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniformFloat(const std::string& name, float value) const
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniformFloat2(const std::string& name, const glm::vec2& values) const
{
	glUniform2f(GetUniformLocation(name), values.x, values.y);
}

void Shader::SetUniformFloat3(const std::string& name, const glm::vec3& values) const
{
	glUniform3f(GetUniformLocation(name), values.r, values.g, values.b);
}

void Shader::SetUniformFloat4(const std::string& name, const glm::vec4& values) const
{
	glUniform4f(GetUniformLocation(name), values.r, values.g, values.b, values.a);
}

void Shader::SetUniformMat3(const std::string& name, const glm::mat3& matrix) const
{
	glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix) const
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}



