#include <cassert>
#include <fstream>
#include <iostream>
#include <QtGui\qmouseevent>
#include <QtGui\qkeyevent>
#include <gtx\transform.hpp>
#include <Renderer.h>
#include <VertexTest.h>
using std::string;



Renderer* Renderer::instance = 0;


Renderer::Renderer()
{
	nextGeometryIndex = 0;
	nextRenderableIndex = 0;
	nextShaderProgramIndex = 0;
}

void Renderer::initializeGL()
{
	setMouseTracking(true);
	glEnable(GL_DEPTH_TEST);
	//	glEnable(GL_CULL_FACE);
	glewInit();
	initializeBuffer();
}

void Renderer::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width(), height());
	//	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	glm::mat4 worldToProjection =
		glm::perspective(60.0f, ((float)width()) / height(), 0.01f, 20.0f) *
		camera.getWorldToViewMatrix();

	for (GLuint i = 0; i < nextRenderableIndex; i++)
	{
		const Renderable* victim = renderables + i;
		const Geometry* g = victim->geometry;

		glBindBuffer(GL_ARRAY_BUFFER, g->buffer->bufferID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			sizeof(VertexTest), (void*)(g->vertexDataBufferByteOffset));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
			sizeof(VertexTest), (void*)(g->vertexDataBufferByteOffset + sizeof(float) * 3));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
			sizeof(VertexTest), (void*)(g->vertexDataBufferByteOffset + sizeof(float) * 6));
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,
			sizeof(VertexTest), (void*)(g->vertexDataBufferByteOffset + sizeof(float) * 9));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g->buffer->bufferID);

		glUseProgram(victim->shaderProgramInfo->programID);
		GLuint mvpLocation = glGetUniformLocation(victim->shaderProgramInfo->programID, "mvp");

		if (mvpLocation != -1)
		{
			glm::mat4 mvp = worldToProjection * victim->modelToWorld;
			glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]);
		}

		// ambient light
		GLint ambientLightUniformLocation = glGetUniformLocation(victim->shaderProgramInfo->programID, "ambientLight");
		glm::vec3 ambientLight(0.3f, 0.0f, 0.0f);
		glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);
		// diffuse light
		GLint diffuseLightPositionUniformLocation = glGetUniformLocation(victim->shaderProgramInfo->programID, "diffuseLightPosition");
		glUniform3fv(diffuseLightPositionUniformLocation, 1, &lightPosition[0]);
		// for specular
		GLint eyePositionWorldUniformLocation = glGetUniformLocation(victim->shaderProgramInfo->programID, "eyePositionWorld");
		glm::vec3 eyePosition = camera.getPosition();
		glUniform3fv(eyePositionWorldUniformLocation, 1, &eyePosition[0]);

		GLint modelToWorldUniformTransform = glGetUniformLocation(victim->shaderProgramInfo->programID, "modelToWorldTransform");
		glUniformMatrix4fv(modelToWorldUniformTransform, 1, GL_FALSE, &victim->modelToWorld[0][0]);

		glUseProgram(victim->shaderProgramInfo->programID);

		glDrawElements(GL_TRIANGLES, victim->geometry->numIndices, GL_UNSIGNED_SHORT,
			(void*)(victim->geometry->indexDataBufferByteOffset));
	}

}

void Renderer::initializeBuffer()
{
	bufferInfo.nextAvailableByteIndex = 0;
	glGenBuffers(1, &bufferInfo.bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferInfo.bufferID);
	glBufferData(GL_ARRAY_BUFFER, BufferInfo::MAX_BUFFER_SIZE, 0, GL_DYNAMIC_DRAW);
}

Geometry* Renderer::addGeometry(void* verts, uint vertexDataSizeBytes,
	void* indices, uint numIndices, //DataType indiceDataType,
	GLuint indexingMode)//, VertexLayoutInfo* vertexLayoutInfo)
{
	GLuint indexDataBytesRequired = numIndices * sizeof(GLushort);
	GLuint totalBytesRequired = vertexDataSizeBytes + indexDataBytesRequired;
	assert(bufferInfo.nextAvailableByteIndex + totalBytesRequired < BufferInfo::MAX_BUFFER_SIZE);

	assert(nextGeometryIndex != MAX_GEOMETRIES);
	Geometry* ret = geometries + nextGeometryIndex;
	nextGeometryIndex++;

	ret->buffer = &bufferInfo;
	ret->numIndices = numIndices;
	glBindBuffer(GL_ARRAY_BUFFER, bufferInfo.bufferID);
	ret->vertexDataBufferByteOffset = bufferInfo.nextAvailableByteIndex;
	glBufferSubData(GL_ARRAY_BUFFER, ret->vertexDataBufferByteOffset,
		vertexDataSizeBytes, verts);
	bufferInfo.nextAvailableByteIndex += vertexDataSizeBytes;

	ret->indexDataBufferByteOffset = bufferInfo.nextAvailableByteIndex;
	glBufferSubData(GL_ARRAY_BUFFER, ret->indexDataBufferByteOffset,
		indexDataBytesRequired, indices);
	bufferInfo.nextAvailableByteIndex += indexDataBytesRequired;

	//	ret->indexDataBufferByteOffset = bufferInfo.nextAvailableByteIndex;


	return ret;
}
Renderable * Renderer::addRenderable(
	const Geometry* what,
	const glm::mat4& where,
	const ShaderProgramInfo* how)
{
	assert(nextRenderableIndex != MAX_RENDERABLES);
	Renderable* ret = renderables + nextRenderableIndex;
	nextRenderableIndex++;

	ret->geometry = what;
	ret->modelToWorld = where;
	ret->shaderProgramInfo = how;

	return ret;
}

ShaderProgramInfo* Renderer::addShaderProgram(
	const char* vertexShaderFileName,
	const char* fragmentShaderFileName)
{
	assert(nextShaderProgramIndex != MAX_GEOMETRIES);

	ShaderProgramInfo* ret = shaderProgramInfos + nextShaderProgramIndex;
	nextShaderProgramIndex++;

	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	std::string temp = readShaderCode("VertexShaderTest.glsl");//vertexShaderFileName);
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderTest.glsl");//fragmentShaderFileName);
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		goto BADNESSHAPPENED;

	ret->programID = glCreateProgram();
	glAttachShader(ret->programID, vertexShaderID);
	glAttachShader(ret->programID, fragmentShaderID);

	glLinkProgram(ret->programID);

	if (!checkProgramStatus(ret->programID))
		goto BADNESSHAPPENED;

	//	glDeleteShader(vertexShaderID);
	//	glDeleteShader(fragmentShaderID);
	//return ShaderInfo();

	return ret;

BADNESSHAPPENED:
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

bool Renderer::checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;
		delete[] buffer;
		return false;
	}
	return true;
}

bool Renderer::checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool Renderer::checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

std::string Renderer::readShaderCode(const char* fileName)
{
	std::ifstream meInput(fileName);
	if (!meInput.good())
	{
		std::cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());
}

void Renderer::mouseMoveEvent(QMouseEvent* e)
{
	camera.mouseUpdate(glm::vec2(e->x(), e->y()));
	//	std::cout << "got mouse!" << std::endl;
	repaint();
}

void Renderer::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key::Key_W:
		camera.moveForward();
		break;
	case Qt::Key::Key_S:
		camera.moveBackward();
		break;
	case Qt::Key::Key_A:
		camera.strafeLeft();
		break;
	case Qt::Key::Key_D:
		camera.strafeRight();
		break;
	case Qt::Key::Key_R:
		camera.moveUp();
		break;
	case Qt::Key::Key_F:
		camera.moveDown();
		break;
	case Qt::Key::Key_Down:
		lightPosition += glm::vec3(0.0f, 0.0f, 0.2f);
		break;
	case Qt::Key::Key_Up:
		lightPosition += glm::vec3(0.0f, 0.0f, -0.2f);
		break;
	case Qt::Key::Key_Left:
		lightPosition += glm::vec3(-0.2f, 0.0f, 0.0f);
		break;
	case Qt::Key::Key_Right:
		lightPosition += glm::vec3(0.2f, 0.0f, 0.0f);
		break;
	case Qt::Key::Key_Q:
		lightPosition += glm::vec3(0.0f, -0.2f, 0.0f);
		break;
	case Qt::Key::Key_E:
		lightPosition += glm::vec3(0.0f, 0.2f, 0.0f);
		break;
	}
	//	std::cout << "got key!" << std::endl;
	repaint();
}

Renderer::~Renderer()
{
}