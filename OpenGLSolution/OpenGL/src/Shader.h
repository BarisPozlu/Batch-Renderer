#pragma once

#include <string>

class Shader
{
public:
	Shader(const std::string& filePath);
	~Shader();
	void Use() const;
	int GetUniformLocation(const std::string& name);

private:
	void CreateShader(const std::string& filePath);
	void GetShaderSource(const std::string& filePath, std::string& outVertexShader, std::string& outFragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	
private:
	unsigned int m_program;
};
