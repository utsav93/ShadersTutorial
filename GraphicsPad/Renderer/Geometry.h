#pragma once
#include <gl\glew.h>
struct BufferInfo;

struct Geometry
{
	const BufferInfo* buffer;
	GLuint vertexDataBufferByteOffset;
	GLuint indexDataBufferByteOffset;
	GLuint numIndices;
};

