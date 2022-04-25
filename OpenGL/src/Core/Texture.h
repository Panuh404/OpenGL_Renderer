#pragma once

#include <glad/glad.h>
#include <iostream>

class Texture
{
private:
	uint32_t m_RendererID;
	GLenum m_InternalFormat, m_DataFormat;
	std::string m_Filepath;
	unsigned char* m_LocalBuffer;
	int m_Height, m_Width, m_Channels;

public:
	Texture(const std::string& path);
	~Texture();

	void Bind(uint32_t slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline int GetChannels() const { return m_Channels; }
};