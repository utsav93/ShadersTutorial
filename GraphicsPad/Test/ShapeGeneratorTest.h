#pragma once
#include <Test\ShapeDataTest.h>
typedef unsigned int uint;

class ShapeGeneratorTest
{
	static ShapeDataTest makePlaneVerts(uint dimensions);
	static ShapeDataTest makePlaneIndices(uint dimensions);
	static ShapeDataTest makePlaneUnseamedIndices(uint tesselation);

	// Teapot helpers
	static void generatePatches(float * v, float * n, float * tc, unsigned short* el, int grid);
	static void moveLid(int grid, float *v, glm::mat4 lidTransform);
	static void buildPatchReflect(int patchNum,
		float *B, float *dB,
		float *v, float *n,
		float *tc, unsigned short *el,
		int &index, int &elIndex, int &tcIndex, int grid,
		bool reflectX, bool reflectY);
	static void buildPatch(glm::vec3 patch[][4],
		float *B, float *dB,
		float *v, float *n, float *tc,
		unsigned short *el,
		int &index, int &elIndex, int &tcIndex,
		int grid, glm::mat3 reflect,
		bool invertNormal);
	static void getPatch(int patchNum, glm::vec3 patch[][4], bool reverseV);
	static void computeBasisFunctions(float * B, float * dB, int grid);
	static glm::vec3 evaluate(int gridU, int gridV, float *B, glm::vec3 patch[][4]);
	static glm::vec3 evaluateNormal(int gridU, int gridV,
		float *B, float *dB, glm::vec3 patch[][4]);
public:
	static ShapeDataTest makeTriangle();
	static ShapeDataTest makeCube();
	static ShapeDataTest makeArrow();
	static ShapeDataTest makePlane(uint dimensions = 10);
	static ShapeDataTest makeTeapot(uint tesselation = 10, const glm::mat4& lidTransform = glm::mat4());
	static ShapeDataTest makeSphere(uint tesselation = 20);
	static ShapeDataTest makeTorus(uint tesselation = 20);
	static ShapeDataTest generateNormals(const ShapeDataTest& data);
};

