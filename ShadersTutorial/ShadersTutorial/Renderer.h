#pragma once

#include <QtOpenGL\qglwidget>
#include <Renderable.h>
#include <Geometry.h>
#include <BufferInfo.h>

class Renderer : public QGLWidget
{
	static Renderer* instance;

	BufferInfo bufferInfo;

	Renderer();
	Renderer(Renderer&);
	Renderer& operator=(Renderer&);
protected:
	void initialize();
	void initializeBuffer();

public:
	Geometry* addGeometry(void* verts, uint vertexDataSize, void* indices, uint numIndices, GLuint indexingMode);
	Renderable* addRenderable(Geometry* geometry);
};

