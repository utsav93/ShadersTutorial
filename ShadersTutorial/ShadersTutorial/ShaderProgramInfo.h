#pragma once
#include <gl/glew.h>
struct ShaderProgramInfo
{
	GLuint programID;
	ShaderProgramInfo() : programID(-1) {}
};