#include <gl\glew.h>
#include <Renderer.h>


Renderer* Renderer::instance;


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

Geometry* Renderer::addGeometry(void * verts, uint vertexDataSize, void* indices, uint numIndices, GLuint indexingMode)
{


	return nullptr;
}

Renderable* Renderer::addRenderable(Geometry * geometry)
{
	return nullptr;
}

