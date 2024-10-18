#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#ifdef _DEBUG

#define SETUP_GL_DEBUG() glEnable(GL_DEBUG_OUTPUT); \
glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); \
glDebugMessageCallback(DebugMessageCallBack, nullptr); \
glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE)

#define SETUP_GLFW_DEBUG_CONTEXT() glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true)

#else

#define SETUP_GL_DEBUG()
#define SETUP_GLFW_DEBUG_CONTEXT()

#endif


void APIENTRY DebugMessageCallBack(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam);
