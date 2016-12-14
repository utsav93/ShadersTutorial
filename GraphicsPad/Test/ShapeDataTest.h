#pragma once
#include <GL\glew.h>
#include <Test\VertexTest.h>

struct ShapeDataTest
{
	ShapeDataTest() :
		vertices(0), numVertices(0),
		indices(0), numIndices(0) {}
	VertexTest* vertices;
	GLuint numVertices;
	GLushort* indices;
	GLuint numIndices;
	GLsizeiptr vertexBufferSize() const
	{
		return numVertices * sizeof(VertexTest);
	}
	GLsizeiptr indexBufferSize() const
	{
		return numIndices * sizeof(GLushort);
	}
	void cleanup()
	{
		delete [] vertices;
		delete [] indices;
		numVertices = numIndices = 0;
	}
};