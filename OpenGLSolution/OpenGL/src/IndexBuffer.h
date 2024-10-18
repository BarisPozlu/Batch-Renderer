#pragma once

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();
	void Bind() const;
	void Unbind() const;

public:
	inline unsigned int GetCount() const { return m_count; }

private:
	unsigned int m_renderID;
	unsigned int m_count;
};
