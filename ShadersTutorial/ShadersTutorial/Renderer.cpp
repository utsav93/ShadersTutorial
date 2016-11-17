#include <gl\glew.h>
#include <fstream>
#include <string>
#include <iostream>
#include <cassert>
#include <Renderer.h>
using namespace std;



Renderer* Renderer::instance = 0;

//read shader code from files
string Renderer::readShaderCode(const char* fileName)
{
	ifstream myInput(fileName);
	if (!myInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(myInput),
		std::istreambuf_iterator<char>());
}

//error checking code
void Renderer::checkGlProgram(GLuint prog, const char *file, int line)
{
	GLint status;
	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		int loglen;
		char logbuffer[1000];
		glGetProgramInfoLog(prog, sizeof(logbuffer), &loglen, logbuffer);
		fprintf(stderr, "OpenGL Program Linker Error at %s:%d:\n%.*s", file, line, loglen, logbuffer);
	}
	else {
		int loglen;
		char logbuffer[1000];
		glGetProgramInfoLog(prog, sizeof(logbuffer), &loglen, logbuffer);
		if (loglen > 0) {
			fprintf(stderr, "OpenGL Program Link OK at %s:%d:\n%.*s", file, line, loglen, logbuffer);
		}
		glValidateProgram(prog);
		glGetProgramInfoLog(prog, sizeof(logbuffer), &loglen, logbuffer);
		if (loglen > 0) {
			fprintf(stderr, "OpenGL Program Validation results at %s:%d:\n%.*s", file, line, loglen, logbuffer);
		}
	}
}

//shader compile check
bool Renderer::checkShaderStatus(GLuint shaderID)
{
	GLint compileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		GLint infoLogLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];
		GLsizei bufferSize;
		glGetShaderInfoLog(shaderID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
}

ShaderProgramInfo* Renderer::addShaderProgram(const char * vertexShaderFileName, const char * fragmentShaderFileName)
{
	ShaderProgramInfo* ret = shaderProgramInfos + nextShaderProgramIndex;
	nextShaderProgramIndex++;
	assert(nextShaderProgramIndex != MAX_GEOMETRIES);
	
	//Vertex Shader Object
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

	//Fragment Shader Object
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//array of pointers
	const char* adapter[1];
	string temp = readShaderCode(vertexShaderFileName);
	//points to character string(vertex) defined in shadercode.cpp
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode(fragmentShaderFileName);
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
	{
		cout << "error compiling shaders" << endl;
		goto shaderError;
	}

	ret->programID = glCreateProgram();
	glAttachShader(ret->programID, vertexShaderID);
	glAttachShader(ret->programID, fragmentShaderID);

	glLinkProgram(ret->programID);

	checkGlProgram(ret->programID, __FILE__, __LINE__);


	glUseProgram(ret->programID);
	return ret;

shaderError:
	if (vertexShaderID)
		glDeleteShader(vertexShaderID);
	if (fragmentShaderID)
		glDeleteShader(fragmentShaderID);
	if (ret->programID)
	{
		glDeleteProgram(ret->programID);
		ret->programID = 0;
	}
	nextShaderProgramIndex--;
	return 0;
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

	assert(nextGeometryIndex != MAX_GEOMETRIES);
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
	assert(nextRenderableIndex != MAX_RENDERABLES);
	Renderable* ret = renderables + nextRenderableIndex;
	nextRenderableIndex++;

	ret->geometry = geometry;
	ret->modelToWorld = modelToWorldMatrix;
	ret->shaderProgramInfo = shaderProgramInfo;

	return ret;
}

