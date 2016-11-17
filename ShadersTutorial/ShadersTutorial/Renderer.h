#pragma once
#include <gl\glew.h>
#include <QtOpenGL\qglwidget>
#include <Renderable.h>
#include <Geometry.h>
#include <BufferInfo.h>
#include <ShaderProgramInfo.h>

class Renderer : public QGLWidget
{
	static Renderer* instance;

	BufferInfo bufferInfo;

	const static GLuint MAX_GEOMETRIES = 20;
	GLuint nextGeometryIndex;
	Geometry geometries[MAX_GEOMETRIES];

	const static GLuint MAX_RENDERABLES = 100;
	GLuint nextRenderableIndex;
	Renderable renderables[MAX_RENDERABLES];

	const static GLuint MAX_SHADER_PROGRAM_INFOS = 20;
	GLuint nextShaderProgramIndex;
	ShaderProgramInfo shaderProgramInfo[MAX_SHADER_PROGRAM_INFOS];


	Renderer();
	Renderer(Renderer&);
	Renderer& operator=(Renderer&);
protected:
	void initialize();
	void initializeBuffer();

public:
	Geometry* addGeometry(void* verts, uint vertexDataSize, void* indices, uint numIndices, GLuint indexingMode);
	Renderable* addRenderable(const Geometry* geometry = 0, const glm::mat4& modelToWorldMatrix = glm::mat4(), const ShaderProgramInfo* shaderProgramInfo = 0);
	ShaderProgramInfo addShaderProgram(const char* vertexShaderFileName, const char* fragmentShaderFileName);
	static Renderer& getInstance()
	{
		if (instance == 0)
			instance = new Renderer();
		return *instance;
	}
};

#define renderer Renderer::getInstance()
