#include "Shader.h"
#include <GL\glew.h>
#include <sstream>
#include <fstream>


Shader::Shader(const std::string& filePath)
{
    CreateShader(filePath);
}

Shader::~Shader()
{
    glDeleteProgram(m_program);
}

void Shader::GetShaderSource(const std::string& filePath, std::string& outVertexShader, std::string& outFragmentShader)
{
    std::ifstream stream(filePath);
    std::string line;
    std::stringstream ss[2];

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ShaderType type = ShaderType::NONE;

    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }

            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }

        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    outVertexShader = ss[0].str();
    outFragmentShader = ss[1].str();
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    return id;
}


void Shader::CreateShader(const std::string& filePath)
{
    unsigned int program = glCreateProgram();

    std::string vertexShader;
    std::string fragmentShader;

    GetShaderSource(filePath, vertexShader, fragmentShader);

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    
    m_program = program;
}

void Shader::Use() const
{
    glUseProgram(m_program);
}

int Shader::GetUniformLocation(const std::string& name)
{
    return glGetUniformLocation(m_program, name.c_str());
}
