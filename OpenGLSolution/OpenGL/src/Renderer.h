#pragma once

#include "glm/glm.hpp"

class VertexArray;
class IndexBuffer;
class Shader;

class Renderer
{
public:
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void Init();
	void DrawQuad(const glm::vec2& position, const glm::vec2& size, int textureID);
	void DrawQuad(const glm::vec2& position, const glm::vec2& size, glm::vec4 color);
	void Flush();
	void ShutDown();
	void Clear() const;
};
