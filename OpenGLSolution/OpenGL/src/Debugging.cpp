#include "Debugging.h"
#include <iostream>

void DebugMessageCallBack(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
	if (id == 131185) return;

	std::cout << "id: " << id << " message: " << message;
	__debugbreak();
}
