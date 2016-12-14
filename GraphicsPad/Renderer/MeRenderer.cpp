//#include <GL\glew.h>


#include <cassert>
#include <fstream>
#include <iostream>
#include <QtGui\qmouseevent>
#include <QtGui\qkeyevent>
#include <glm\gtx\transform.hpp>
#include <Renderer\MeRenderer.h>
#include <Primitives\Vertex.h>
using std::string;



MeRenderer* MeRenderer::instance = 0;


MeRenderer::MeRenderer()
{
	nextGeometryIndex = 0;
	nextRenderableIndex = 0;
	nextShaderProgramIndex = 0;

//	myNormal1 = QGLWidget::convertToGLFormat(QImage(normalName1, "PNG"));
}

void MeRenderer::initializeGL()
{
	setMouseTracking(true);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glewInit();
	initializeBuffer();	
}

void MeRenderer::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width(), height());
//	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	glm::mat4 worldToProjection = 
		glm::perspective(60.0f, ((float)width())/height(), 0.01f, 20.0f) * 
		camera.getWorldToViewMatrix();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (GLuint i = 0; i < nextRenderableIndex; i++)
	{
		const Renderable* victim = renderables + i;
		const Geometry* g = victim->geometry;

		glBindBuffer(GL_ARRAY_BUFFER, g->buffer->bufferID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (void*)(g->vertexDataBufferByteOffset));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (void*)(g->vertexDataBufferByteOffset + sizeof(float) * 3));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (void*)(g->vertexDataBufferByteOffset + sizeof(float) * 6));
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (void*)(g->vertexDataBufferByteOffset + sizeof(float) * 9));
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (void*)(g->vertexDataBufferByteOffset + sizeof(float) * 11));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g->buffer->bufferID);

		

		glUseProgram(victim->shaderProgramInfo->programID);
		GLuint mvpLocation = glGetUniformLocation(victim->shaderProgramInfo->programID, "mvp");

		if (mvpLocation != -1)
		{
			if (victim->isLight == true)
			{
				glm::mat4 lightModelToWorld = glm::mat4(glm::translate(lightPosition) * glm::scale(0.1f, 0.1f, 0.1f));
				glm::mat4 mvp = worldToProjection * lightModelToWorld;
				glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]);
			}
			else {
				glm::mat4 mvp = worldToProjection * victim->modelToWorld;
				glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]);
			}				
		}

		glm::mat4 modelToWorldInverseTrans = glm::inverse(glm::transpose(victim->modelToWorld));
		GLint modelToWorldInvertTransUniformLocation =
			glGetUniformLocation(victim->shaderProgramInfo->programID, "modelToWorldInvertTrans");
		glUniformMatrix4fv(modelToWorldInvertTransUniformLocation, 1, GL_FALSE,
			&modelToWorldInverseTrans[0][0]);

 //   	QImage myImg;
    	GLuint myTextureObjectId;
//    	QImage myNormal;
//    	GLuint myNormalObjectId;
//    	QImage mySpec;
//    	GLuint mySpecObjectId;
//    	QImage myAlpha;
//    	GLuint myAlphaObjectId;
		
//		QImage myImg = *(victim->myTex);
//		glActiveTexture(GL_TEXTURE0);
//		glGenTextures(1, &myTextureObjectId);
//		glBindTexture(GL_TEXTURE_2D, myTextureObjectId);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, myImg.width(), myImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, myImg.bits());
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//		glActiveTexture(GL_TEXTURE1);
//		glGenTextures(1, &myAlphaObjectId);
//		glBindTexture(GL_TEXTURE_2D, myAlphaObjectId);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, victim->myAlpha.width(), victim->myAlpha.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, victim->myAlpha.bits());
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//		glActiveTexture(GL_TEXTURE2);
//		glGenTextures(1, &myNormalObjectId);
//		glBindTexture(GL_TEXTURE_2D, myNormalObjectId);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, victim->myNormal.width(), victim->myNormal.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, victim->myNormal.bits());
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//		glActiveTexture(GL_TEXTURE3);
//		glGenTextures(1, &mySpecObjectId);
//		glBindTexture(GL_TEXTURE_2D, mySpecObjectId);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, victim->mySpec.width(), victim->mySpec.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, victim->mySpec.bits());
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
		int locTex = glGetUniformLocation(victim->shaderProgramInfo->programID, "Tex1");
		if (locTex >= 0) glUniform1i(locTex, 0);
		int locAlpha = glGetUniformLocation(victim->shaderProgramInfo->programID, "Alpha1");
		if (locAlpha >= 0) glUniform1i(locAlpha, 1);
		int locN = glGetUniformLocation(victim->shaderProgramInfo->programID, "Normal1");
		if (locN >= 0) glUniform1i(locN, 2);
		int locSpec = glGetUniformLocation(victim->shaderProgramInfo->programID, "Spec1");
		if (locSpec >= 0) glUniform1i(locSpec, 3);
		int locAo = glGetUniformLocation(victim->shaderProgramInfo->programID, "Ao1");
		if (locAo >= 0) glUniform1i(locAo, 4);

		// ambient light
		GLint ambientLightUniformLocation = glGetUniformLocation(victim->shaderProgramInfo->programID, "ambientLight");
		glm::vec3 ambientLight(0.5f, 0.1f, 0.1f);
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

void MeRenderer::initializeBuffer()
{
	bufferInfo.nextAvailableByteIndex = 0;
	glGenBuffers(1, &bufferInfo.bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferInfo.bufferID);
	glBufferData(GL_ARRAY_BUFFER, BufferInfo::MAX_BUFFER_SIZE, 0, GL_DYNAMIC_DRAW);
}

Geometry* MeRenderer::addGeometry(void* verts, uint vertexDataSizeBytes,
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
Renderable * MeRenderer::addRenderable(
	const Geometry* what, 
	const glm::mat4& where, 
	const ShaderProgramInfo* how,
	const char* texName,
	const char* alphaName, 
	const char* normalName, 
	const char* specName, 
	const char* aoName )
{
	assert(nextRenderableIndex != MAX_RENDERABLES);
	Renderable* ret = renderables + nextRenderableIndex;
	nextRenderableIndex++;

	ret->geometry = what;
	ret->modelToWorld = where;
	ret->shaderProgramInfo = how;

//	QImage myImg;
//	GLuint myTextureObjectId;
////	QImage myNormal;
//	GLuint myNormalObjectId;
////	QImage mySpec;
//	GLuint mySpecObjectId;
////	QImage myAlpha;
//	GLuint myAlphaObjectId;

	QImage myImg;
	GLuint myTextureObjectId;
	QImage myAlpha;
	GLuint myAlphaObjectId;
	QImage myNormal;
	GLuint myNormalObjectId;
	QImage mySpec;
	GLuint mySpecObjectId;
	QImage myAo;
	GLuint myAoObjectId;

	myImg = QGLWidget::convertToGLFormat(QImage(texName, "PNG"));
//	ret->myTex = &myImg;
//	*(ret->myTex) = myImg;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &myTextureObjectId);
	glBindTexture(GL_TEXTURE_2D, myTextureObjectId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, myImg.width(), myImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, myImg.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	myAlpha = QGLWidget::convertToGLFormat(QImage(alphaName, "PNG"));
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &myAlphaObjectId);
	glBindTexture(GL_TEXTURE_2D, myAlphaObjectId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, myAlpha.width(), myAlpha.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, myAlpha.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	myNormal = QGLWidget::convertToGLFormat(QImage(normalName, "PNG"));
	glActiveTexture(GL_TEXTURE2);
	glGenTextures(1, &myNormalObjectId);
	glBindTexture(GL_TEXTURE_2D, myNormalObjectId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, myNormal.width(), myNormal.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, myNormal.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	mySpec = QGLWidget::convertToGLFormat(QImage(specName, "PNG"));
	glActiveTexture(GL_TEXTURE3);
	glGenTextures(1, &mySpecObjectId);
	glBindTexture(GL_TEXTURE_2D, mySpecObjectId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mySpec.width(), mySpec.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, mySpec.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	myAo = QGLWidget::convertToGLFormat(QImage(aoName, "PNG"));
	glActiveTexture(GL_TEXTURE4);
	glGenTextures(1, &myAoObjectId);
	glBindTexture(GL_TEXTURE_2D, myAoObjectId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, myAo.width(), myAo.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, myAo.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

//	int locTex = glGetUniformLocation(ret->shaderProgramInfo->programID, "Tex1");
//	if (locTex >= 0) glUniform1i(locTex, 0);
//	int locAlpha = glGetUniformLocation(ret->shaderProgramInfo->programID, "Alpha1");
//	if (locAlpha >= 0) glUniform1i(locAlpha, 1);
//	int locN = glGetUniformLocation(ret->shaderProgramInfo->programID, "Normal1");
//	if (locN >= 0) glUniform1i(locN, 2);
//	int locSpec = glGetUniformLocation(ret->shaderProgramInfo->programID, "Spec1");
//	if (locSpec >= 0) glUniform1i(locSpec, 3);			

	return ret;
}

Renderable * MeRenderer::addLight(
	const Geometry* what,
	const glm::mat4& where,
	const ShaderProgramInfo* how)
{
	assert(nextRenderableIndex != MAX_RENDERABLES);
	Renderable* ret = renderables + nextRenderableIndex;
	nextRenderableIndex++;

	ret->geometry = what;
//	ret->modelToWorld = where;
	ret->modelToWorld = glm::mat4(glm::translate(lightPosition) * glm::scale(0.1f, 0.1f, 0.1f));
	ret->shaderProgramInfo = how;
	ret->isLight = true;

	return ret;
}

ShaderProgramInfo* MeRenderer::addShaderProgram(
	const char* vertexShaderFileName, 
	const char* fragmentShaderFileName)
{
	assert(nextShaderProgramIndex != MAX_GEOMETRIES);

	ShaderProgramInfo* ret = shaderProgramInfos + nextShaderProgramIndex;
	nextShaderProgramIndex++;

	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	std::string temp = readShaderCode(vertexShaderFileName);
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode(fragmentShaderFileName);
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

bool MeRenderer::checkStatus(
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

bool MeRenderer::checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool MeRenderer::checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

std::string MeRenderer::readShaderCode(const char* fileName)
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

void MeRenderer::mouseMoveEvent(QMouseEvent* e)
{
	camera.mouseUpdate(glm::vec2(e->x(), e->y()));
//	std::cout << "got mouse!" << std::endl;
	repaint();
}

void MeRenderer::keyPressEvent(QKeyEvent* e)
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

MeRenderer::~MeRenderer()
{
}