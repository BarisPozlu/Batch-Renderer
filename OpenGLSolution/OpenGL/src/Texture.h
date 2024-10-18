#pragma once

#include <string>

class Texture
{
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0);
	void Unbind();

public:
	inline unsigned int GetID() { return m_renderID; }

private:
	unsigned int m_renderID;
	int m_width;
	int m_height;
	int m_BPP;
};
