#pragma once
//#include <gl\glew.h>
struct BufferInfo
{
	static const GLuint MAX_BUFFER_SIZE = 10000000;
	GLuint bufferID;
	GLuint nextAvailableByteIndex;
};
