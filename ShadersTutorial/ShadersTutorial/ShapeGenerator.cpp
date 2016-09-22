#include "ShapeGenerator.h"
#include <glm.hpp>
#include <Vertex.h>
#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)


ShapeData ShapeGenerator::makeTriangle()
{
	ShapeData ret;
	Vertex tri[] =
	{
		glm::vec3(+0.0f, +1.0f, +0.0f),
		glm::vec3(+1.0f, +0.0f, +0.0f),

		glm::vec3(-1.0f, -1.0f, +0.0f),
		glm::vec3(+0.0f, +1.0f, +0.5f),

		glm::vec3(+1.0f, -1.0f, +0.0f),
		glm::vec3(+0.0f, +0.0f, +1.0f),

	};
	ret.numVertices = NUM_ARRAY_ELEMENTS(tri);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, tri, sizeof(tri));

	GLushort indices[] = { 0,1,2 };
	ret.numIndices = NUM_ARRAY_ELEMENTS(indices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, indices, sizeof(indices));

	return ret;
}

ShapeData ShapeGenerator::makeCube() {
	ShapeData ret;
	Vertex stackVerts[] =
	{
		glm::vec3(-1.0f, +1.0f, +1.0f),  // 0
		glm::vec3(+1.0f, +0.0f, +0.0f),	// Color

		glm::vec3(+1.0f, +1.0f, +1.0f),  // 1
		glm::vec3(+0.0f, +1.0f, +0.0f),	// Color

		glm::vec3(+1.0f, +1.0f, -1.0f),  // 2
		glm::vec3(+0.0f, +0.0f, +1.0f),  // Color

		glm::vec3(-1.0f, +1.0f, -1.0f),  // 3
		glm::vec3(+1.0f, +1.0f, +1.0f),  // Color


		glm::vec3(-1.0f, +1.0f, -1.0f),  // 4
		glm::vec3(+1.0f, +0.0f, +1.0f),  // Color

		glm::vec3(+1.0f, +1.0f, -1.0f),  // 5
		glm::vec3(+0.0f, +0.5f, +0.2f),  // Color

		glm::vec3(+1.0f, -1.0f, -1.0f),  // 6
		glm::vec3(+0.8f, +0.6f, +0.4f),  // Color

		glm::vec3(-1.0f, -1.0f, -1.0f),  // 7
		glm::vec3(+0.3f, +1.0f, +0.5f),  // Color


		glm::vec3(+1.0f, +1.0f, -1.0f),  // 8
		glm::vec3(+0.2f, +0.5f, +0.2f),  // Color

		glm::vec3(+1.0f, +1.0f, +1.0f),  // 9
		glm::vec3(+0.9f, +0.3f, +0.7f),  // Color

		glm::vec3(+1.0f, -1.0f, +1.0f),  // 10
		glm::vec3(+0.3f, +0.7f, +0.5f),  // Color

		glm::vec3(+1.0f, -1.0f, -1.0f),  // 11
		glm::vec3(+0.5f, +0.7f, +0.5f),  // Color


		glm::vec3(-1.0f, +1.0f, +1.0f),  // 12
		glm::vec3(+0.7f, +0.8f, +0.2f),  // Color

		glm::vec3(-1.0f, +1.0f, -1.0f),  // 13
		glm::vec3(+0.5f, +0.7f, +0.3f),  // Color

		glm::vec3(-1.0f, -1.0f, -1.0f),  // 14
		glm::vec3(+0.4f, +0.7f, +0.7f),  // Color

		glm::vec3(-1.0f, -1.0f, +1.0f),  // 15
		glm::vec3(+0.2f, +0.5f, +1.0f),  // Color


		glm::vec3(+1.0f, +1.0f, +1.0f),  // 16
		glm::vec3(+0.6f, +1.0f, +0.7f),  // Color

		glm::vec3(-1.0f, +1.0f, +1.0f),  // 17
		glm::vec3(+0.6f, +0.4f, +0.8f),  // Color

		glm::vec3(-1.0f, -1.0f, +1.0f),  // 18
		glm::vec3(+0.2f, +0.8f, +0.7f),  // Color

		glm::vec3(+1.0f, -1.0f, +1.0f),  // 19
		glm::vec3(+0.2f, +0.7f, +1.0f),  // Color


		glm::vec3(+1.0f, -1.0f, -1.0f),  // 20
		glm::vec3(+0.8f, +0.3f, +0.7f),  // Color

		glm::vec3(-1.0f, -1.0f, -1.0f),  // 21
		glm::vec3(+0.8f, +0.9f, +0.5f),  // Color

		glm::vec3(-1.0f, -1.0f, +1.0f),  // 22
		glm::vec3(+0.5f, +0.8f, +0.5f),  // Color

		glm::vec3(+1.0f, -1.0f, +1.0f),  // 23
		glm::vec3(+0.9f, +1.0f, +0.2f),  // Color

	};

	GLushort stackIndices[] = {
		0,   1,  2,  0,  2,  3, // Top
		4,   5,  6,  4,  6,  7, // Front
		8,   9, 10,  8, 10, 11, // Right
		12, 13, 14, 12, 14, 15, // Left
		16, 17, 18, 16, 18, 19, // Back
		20, 22, 21, 20, 23, 22, // Bottom
	};

	ret.numVertices = NUM_ARRAY_ELEMENTS(stackVerts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, stackVerts, sizeof(stackVerts));

	ret.numIndices = NUM_ARRAY_ELEMENTS(stackIndices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, stackIndices, sizeof(stackIndices));
	return ret;
}

ShapeData ShapeGenerator::makeArrow()
{
	ShapeData ret;
	Vertex stackVerts[] =
	{
		// Top side of arrow head
		glm::vec3(+0.00f, +0.25f, -0.25f),         // 0
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
		glm::vec3(+0.50f, +0.25f, -0.25f),         // 1
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
		glm::vec3(+0.00f, +0.25f, -1.00f),         // 2
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
		glm::vec3(-0.50f, +0.25f, -0.25f),         // 3
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
		// Bottom side of arrow head
		glm::vec3(+0.00f, -0.25f, -0.25f),         // 4
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
		glm::vec3(+0.50f, -0.25f, -0.25f),         // 5
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
		glm::vec3(+0.00f, -0.25f, -1.00f),         // 6
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
		glm::vec3(-0.50f, -0.25f, -0.25f),         // 7
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
		// Right side of arrow tip
		glm::vec3(+0.50f, +0.25f, -0.25f),         // 8
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		glm::vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
		glm::vec3(+0.00f, +0.25f, -1.00f),         // 9
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		glm::vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
		glm::vec3(+0.00f, -0.25f, -1.00f),         // 10
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		glm::vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
		glm::vec3(+0.50f, -0.25f, -0.25f),         // 11
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		glm::vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
		// Left side of arrow tip
		glm::vec3(+0.00f, +0.25f, -1.00f),         // 12
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		glm::vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
		glm::vec3(-0.50f, +0.25f, -0.25f),         // 13
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		glm::vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
		glm::vec3(+0.00f, -0.25f, -1.00f),         // 14
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		glm::vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
		glm::vec3(-0.50f, -0.25f, -0.25f),         // 15
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		glm::vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
		// Back side of arrow tip
		glm::vec3(-0.50f, +0.25f, -0.25f),         // 16
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
		glm::vec3(+0.50f, +0.25f, -0.25f),         // 17
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
		glm::vec3(-0.50f, -0.25f, -0.25f),         // 18
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
		glm::vec3(+0.50f, -0.25f, -0.25f),         // 19
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
		// Top side of back of arrow
		glm::vec3(+0.25f, +0.25f, -0.25f),         // 20
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
		glm::vec3(+0.25f, +0.25f, +1.00f),         // 21
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
		glm::vec3(-0.25f, +0.25f, +1.00f),         // 22
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
		glm::vec3(-0.25f, +0.25f, -0.25f),         // 23
		glm::vec3(+1.00f, +0.00f, +0.00f),		  // Color
		glm::vec3(+0.00f, +1.00f, +0.00f),         // Normal
		// Bottom side of back of arrow
		glm::vec3(+0.25f, -0.25f, -0.25f),         // 24
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
		glm::vec3(+0.25f, -0.25f, +1.00f),         // 25
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
		glm::vec3(-0.25f, -0.25f, +1.00f),         // 26
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
		glm::vec3(-0.25f, -0.25f, -0.25f),         // 27
		glm::vec3(+0.00f, +0.00f, +1.00f),		  // Color
		glm::vec3(+0.00f, -1.00f, +0.00f),         // Normal
		// Right side of back of arrow
		glm::vec3(+0.25f, +0.25f, -0.25f),         // 28
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		glm::vec3(+1.00f, +0.00f, +0.00f),         // Normal
		glm::vec3(+0.25f, -0.25f, -0.25f),         // 29
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		glm::vec3(+1.00f, +0.00f, +0.00f),         // Normal
		glm::vec3(+0.25f, -0.25f, +1.00f),         // 30
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		glm::vec3(+1.00f, +0.00f, +0.00f),         // Normal
		glm::vec3(+0.25f, +0.25f, +1.00f),         // 31
		glm::vec3(+0.60f, +1.00f, +0.00f),		  // Color
		glm::vec3(+1.00f, +0.00f, +0.00f),         // Normal
		// Left side of back of arrow
		glm::vec3(-0.25f, +0.25f, -0.25f),         // 32
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		glm::vec3(-1.00f, +0.00f, +0.00f),         // Normal
		glm::vec3(-0.25f, -0.25f, -0.25f),         // 33
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		glm::vec3(-1.00f, +0.00f, +0.00f),         // Normal
		glm::vec3(-0.25f, -0.25f, +1.00f),         // 34
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		glm::vec3(-1.00f, +0.00f, +0.00f),         // Normal
		glm::vec3(-0.25f, +0.25f, +1.00f),         // 35
		glm::vec3(+0.00f, +1.00f, +0.00f),		  // Color
		glm::vec3(-1.00f, +0.00f, +0.00f),         // Normal
		// Back side of back of arrow
		glm::vec3(-0.25f, +0.25f, +1.00f),         // 36
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
		glm::vec3(+0.25f, +0.25f, +1.00f),         // 37
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
		glm::vec3(-0.25f, -0.25f, +1.00f),         // 38
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
		glm::vec3(+0.25f, -0.25f, +1.00f),         // 39
		glm::vec3(+0.50f, +0.50f, +0.50f),		  // Color
		glm::vec3(+0.00f, +0.00f, +1.00f),         // Normal
};

	GLushort stackIndices[] = {
		0, 1, 2, // Top
		0, 2, 3,
		4, 6, 5, // Bottom
		4, 7, 6,
		8, 10, 9, // Right side of arrow tip
		8, 11, 10,
		12, 15, 13, // Left side of arrow tip
		12, 14, 15,
		16, 19, 17, // Back side of arrow tip
		16, 18, 19,
		20, 22, 21, // Top side of back of arrow
		20, 23, 22,
		24, 25, 26, // Bottom side of back of arrow
		24, 26, 27,
		28, 30, 29, // Right side of back of arrow
		28, 31, 30,
		32, 33, 34, // Left side of back of arrow
		32, 34, 35,
		36, 39, 37, // Back side of back of arrow
		36, 38, 39,
	};

	ret.numVertices = sizeof(stackVerts) / sizeof(*stackVerts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, stackVerts, sizeof(stackVerts));

	ret.numIndices = sizeof(stackIndices) / sizeof(*stackIndices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, stackIndices, sizeof(stackIndices));
	return ret;
}