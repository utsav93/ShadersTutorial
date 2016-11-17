#include <gl\glew.h>
#include <fstream>
#include <string>
#include <iostream>
#include <cassert>
#include <Renderer.h>
using std::string;
using std::istream;
using std::ostream;


Renderer* Renderer::instance = 0;

ShaderProgramInfo Renderer::addShaderProgram(const char * vertexShaderFileName, const char * fragmentShaderFileName)
{
	programID = glCreateProgram();
	//Vertex Shader Object
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

	//Fragment Shader Object
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//array of pointers
	const char* adapter[1];
	string temp = readShaderCode("VertexShaderCode.glsl");
	//points to character string(vertex) defined in shadercode.cpp
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		cout << "error compiling shaders" << endl;

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);

	checkGlProgram(programID, __FILE__, __LINE__);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);


	glUseProgram(programID);
	checkGlProgram(programID, __FILE__, __LINE__);
}

Renderer::Renderer()
{
	nextGeometryIndex = 0;
	nextRenderableIndex = 0;
	nextShaderProgramIndex = 0;
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

Renderable* Renderer::addRenderable(const Geometry* geometry, const glm::mat4& modelToWorldMatrix, const ShaderProgramInfo* shaderProgramInfo)
{
	Renderable* ret = renderables + nextRenderableIndex;
	nextRenderableIndex++;

	ret->geometry = geometry;
	ret->modelToWorld = modelToWorldMatrix;
	ret->shaderProgramInfo = shaderProgramInfo;

	return ret;
}

