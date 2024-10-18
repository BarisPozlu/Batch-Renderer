#include "Renderer.h"
#include <GL\glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include <array>


struct Vertex
{
	glm::vec2 position;
	glm::vec4 color;
	glm::vec2 texCoord;
	float texSlot = 0;
};

struct RenderData
{
	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint indexBuffer;
	
	Vertex* vertexData = nullptr;
	Vertex* currentVertex = nullptr;

	GLsizei indicesUsed;

	std::array<unsigned int, 32> textureIDs;
	int currentTextureSlot = 1;
};

static RenderData s_Data;
static constexpr int s_MaxQuadCount = 25;
static constexpr int s_MaxVertexCount = s_MaxQuadCount * 4;
static constexpr int s_MaxIndexCount = s_MaxQuadCount * 6;

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	va.Bind();
	ib.Bind();
	shader.Use();
	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Init()
{
	s_Data.vertexData = new Vertex[s_MaxVertexCount];
	s_Data.currentVertex = s_Data.vertexData;

	glCreateVertexArrays(1, &s_Data.vertexArray);
	glBindVertexArray(s_Data.vertexArray);

	glGenBuffers(1, &s_Data.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, s_Data.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, s_MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texCoord));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texSlot));

	glGenBuffers(1, &s_Data.indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.indexBuffer);

	unsigned int indices[s_MaxIndexCount];

	int offset = 0;
	for (int i = 0; i < s_MaxIndexCount; i += 6)
	{
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;
		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;

		offset += 4;
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, s_MaxIndexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &s_Data.textureIDs[0]);
	glBindTexture(GL_TEXTURE_2D, s_Data.textureIDs[0]);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	uint32_t color = 0xFFFFFFFF;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);
}

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, int textureID)
{
	if (s_Data.indicesUsed >= s_MaxIndexCount || s_Data.currentTextureSlot > 31)
	{
		Flush();
	}

	float textureSlot = 0;

	for (int i = 0; i < s_Data.currentTextureSlot; i++)
	{
		if (s_Data.textureIDs[i] == textureID)
		{
			textureSlot = i;
			break;
		}
	}

	if (textureSlot == 0)
	{
		s_Data.textureIDs[s_Data.currentTextureSlot] = textureID;
		textureSlot = s_Data.currentTextureSlot;
		s_Data.currentTextureSlot++;
	}

	s_Data.currentVertex->position = position;
	s_Data.currentVertex->color = { 1, 1, 1, 1 };
	s_Data.currentVertex->texCoord = { 0, 0 };
	s_Data.currentVertex->texSlot = textureSlot;

	s_Data.currentVertex++;

	s_Data.currentVertex->position = { position.x + size.x, position.y };
	s_Data.currentVertex->color = { 1, 1, 1, 1 };
	s_Data.currentVertex->texCoord = { 1, 0 };
	s_Data.currentVertex->texSlot = textureSlot;

	s_Data.currentVertex++;

	s_Data.currentVertex->position = { position.x + size.x, position.y + size.y };
	s_Data.currentVertex->color = { 1, 1, 1, 1 };
	s_Data.currentVertex->texCoord = { 1, 1 };
	s_Data.currentVertex->texSlot = textureSlot;

	s_Data.currentVertex++;

	s_Data.currentVertex->position = { position.x, position.y + size.y};
	s_Data.currentVertex->color = { 1, 1, 1, 1 };
	s_Data.currentVertex->texCoord = { 0, 1 };
	s_Data.currentVertex->texSlot = textureSlot;

	s_Data.currentVertex++;

	s_Data.indicesUsed += 6;
}

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, glm::vec4 color)
{
	if (s_Data.indicesUsed >= s_MaxIndexCount)
	{
		Flush();
	}

	s_Data.currentVertex->position = position;
	s_Data.currentVertex->texCoord = { 0, 0 };
	s_Data.currentVertex->texSlot = 0;
	s_Data.currentVertex->color = color;

	s_Data.currentVertex++;

	s_Data.currentVertex->position = { position.x + size.x, position.y };
	s_Data.currentVertex->texCoord = { 1, 0 };
	s_Data.currentVertex->texSlot = 0;
	s_Data.currentVertex->color = color;

	s_Data.currentVertex++;

	s_Data.currentVertex->position = { position.x + size.x, position.y + size.y };
	s_Data.currentVertex->texCoord = { 1, 1 };
	s_Data.currentVertex->texSlot = 0;
	s_Data.currentVertex->color = color;

	s_Data.currentVertex++;

	s_Data.currentVertex->position = { position.x, position.y + size.y };
	s_Data.currentVertex->texCoord = { 0, 1 };
	s_Data.currentVertex->texSlot = 0;
	s_Data.currentVertex->color = color;

	s_Data.currentVertex++;

	s_Data.indicesUsed += 6;
}

void Renderer::Flush()
{
	glBindBuffer(GL_ARRAY_BUFFER, s_Data.vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, (uint8_t*)s_Data.currentVertex - (uint8_t*)s_Data.vertexData, s_Data.vertexData);

	for (int i = 0; i < s_Data.currentTextureSlot; i++)
	{
		glBindTextureUnit(i, s_Data.textureIDs[i]);
	}

	glBindVertexArray(s_Data.vertexArray);
	glDrawElements(GL_TRIANGLES, s_Data.indicesUsed, GL_UNSIGNED_INT, nullptr);

	s_Data.currentVertex = s_Data.vertexData;
	s_Data.indicesUsed = 0;
	s_Data.currentTextureSlot = 1;
}

void Renderer::ShutDown()
{
	delete[] s_Data.vertexData;

	glDeleteTextures(1, &s_Data.textureIDs[0]);
	glDeleteVertexArrays(1, &s_Data.vertexArray);
	glDeleteBuffers(1, &s_Data.vertexBuffer);
	glDeleteBuffers(1, &s_Data.indexBuffer);
}

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}
