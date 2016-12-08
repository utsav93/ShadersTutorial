#pragma once
#include <gl\glew.h>
#include <QtOpenGL\qglwidget>
#include <glm.hpp>
#include <Renderable.h>
#include <Geometry.h>
#include <BufferInfo.h>
#include <ShaderProgramInfo.h>
#include <Camera.h>
#include <string>

class Renderer : public QGLWidget
{
	static Renderer* instance;

	Camera camera;

	BufferInfo bufferInfo;

	const static GLuint MAX_GEOMETRIES = 20;
	GLuint nextGeometryIndex;
	Geometry geometries[MAX_GEOMETRIES];

	const static GLuint MAX_RENDERABLES = 100;
	GLuint nextRenderableIndex;
	Renderable renderables[MAX_RENDERABLES];

	const static GLuint MAX_SHADER_PROGRAM_INFOS = 100;
	GLuint nextShaderProgramIndex;
	ShaderProgramInfo shaderProgramInfos[MAX_SHADER_PROGRAM_INFOS];

	void initializeBuffer();

	bool checkShaderStatus(GLuint);
	std::string readShaderCode(const char* fileName);
	bool checkGlProgram(GLuint, const char*, int);


	Renderer();
	Renderer(Renderer&);
	Renderer& operator=(Renderer&);
	~Renderer();
protected:
	void initializeGL();
	void paintGL();

	void mouseMoveEvent(QMouseEvent* e);
	void keyPressEvent(QKeyEvent* e);

public:
	Geometry* addGeometry(void* verts, uint vertexDataSize, void* indices, uint numIndices, GLuint indexingMode);
	Renderable* addRenderable(const Geometry* geometry = 0, const glm::mat4& modelToWorldMatrix = glm::mat4(), const ShaderProgramInfo* shaderProgramInfo = 0);
	ShaderProgramInfo* addShaderProgram(const char* vertexShaderFileName, const char* fragmentShaderFileName);
	inline static Renderer& getInstance()
	{
		if (instance == 0)
			instance = new Renderer();
		return *instance;
	}
};

#define renderer Renderer::getInstance()
