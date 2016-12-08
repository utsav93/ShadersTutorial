#pragma once
#include <gl\glew.h>
struct BufferInfo;
struct ShaderProgramInfo;

struct Geometry
{
	const BufferInfo* buffer;
	GLuint vertexDataBufferByteOffset;
	GLuint indexDataBufferByteOffset;
	GLuint numIndices;
};