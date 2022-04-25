#pragma once

#include "Buffer.h"

class VertexArray
{
private:
	uint32_t m_RendererID;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const;
	void Bind() const;
	void UnBind() const;
};