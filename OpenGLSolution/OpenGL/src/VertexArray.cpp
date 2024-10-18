#include "VertexArray.h"
#include <glew.h>

VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_renderID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_renderID);
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_renderID);
}

void VertexArray::Unbind() const 
{
	glBindVertexArray(0);
}
