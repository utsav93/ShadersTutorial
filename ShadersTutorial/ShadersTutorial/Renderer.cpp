#include <gl\glew.h>
#include <Renderer.h>
#include <cassert>


Renderer* Renderer::instance;

Renderer::Renderer()
{
	nextGeometryIndex = 0;
}


void Renderer::initialize()
{
	glewInit();
	initializeBuffer();
	
}

void Renderer::initializeBuffer()
{
	bufferInfo.nextAvailableByteIndex = 0;
	glGenBuffers(1, &bufferInfo.bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferInfo.bufferID);
	glBufferData(GL_ARRAY_BUFFER, bufferInfo.MAX_BUFFER_SIZE, 0, GL_DYNAMIC_DRAW);
}

Geometry* Renderer::addGeometry(void * verts, uint vertexDataSizeBytes, void* indices, uint numIndices, GLuint indexingMode)
{
	GLuint indexDataSizeBytes = numIndices * sizeof(GLushort);
	GLuint totalBytesRequired = vertexDataSizeBytes + indexDataSizeBytes;
	assert(bufferInfo.nextAvailableByteIndex + totalBytesRequired < BufferInfo::MAX_BUFFER_SIZE);

	Geometry* ret = geometries + nextGeometryIndex;
	nextGeometryIndex++;

	glBindBuffer(GL_ARRAY_BUFFER, bufferInfo.bufferID);

	ret->vertexDataBufferByteOffset = bufferInfo.nextAvailableByteIndex;
	glBufferSubData(GL_ARRAY_BUFFER, ret->vertexDataBufferByteOffset, vertexDataSizeBytes, verts);

	bufferInfo.nextAvailableByteIndex += vertexDataSizeBytes;

	ret->indexDataBufferByteOffset = bufferInfo.nextAvailableByteIndex;
	glBufferSubData(GL_ARRAY_BUFFER, ret->indexDataBufferByteOffset, indexDataSizeBytes, indices);

	bufferInfo.nextAvailableByteIndex += indexDataSizeBytes;

	return ret;
}

Renderable* Renderer::addRenderable(Geometry * geometry)
{
	return nullptr;
}

