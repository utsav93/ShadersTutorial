#pragma once
#include <GL\glew.h>

struct ShaderProgramInfo
{
	GLuint programID;
	ShaderProgramInfo() : programID(-1) {}
};