#include <Qt\qapplication.h>
//#include <MeGlWindow.h>
#include<Renderer.h>
//#include <Primitives\ShapeGenerator.h>
#include <ShapeGeneratorTest.h>
#include <gtx\transform.hpp>

int main(int argc, char* argv[])
{

	QApplication app(argc, argv);
	//	ShapeData data = ShapeGenerator::makeCube();
	ShapeDataTest data = ShapeGeneratorTest::makeCube();
	renderer.show();

	Geometry* cubeGeometry =
		renderer.addGeometry(
			data.vertices, data.vertexBufferSize(),
			data.indices, data.numIndices, GL_TRIANGLES);

	ShaderProgramInfo* shader = renderer.addShaderProgram(
		"VertexShaderTest.glsl",
		"FragmentShaderTest.glsl");

	Renderable* cubeRenderable1 = renderer.addRenderable(cubeGeometry,
		glm::mat4(), shader);
	//	Renderable* cubeRenderable2 = renderer.addRenderable(cubeGeometry,
	//		glm::translate(-2.0f, 0.0f, 0.0f), shader);
	return app.exec();
	system("PAUSE");
}