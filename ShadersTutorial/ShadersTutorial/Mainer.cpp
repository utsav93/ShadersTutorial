#include <Qt\QApplication.h>
#include <gtx\transform.hpp>
#include <Renderer.h>
#include <ShapeGenerator.h>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	ShapeData data = ShapeGenerator::makeCube();

	renderer.show();

	ShaderProgramInfo* shader = renderer.addShaderProgram("vertexShaderCode.glsl", "FragmentShaderCode.glsl");

	Geometry* cubeGeometryData = renderer.addGeometry(data.vertices, data.vertexBufferSize(), data.indices, data.numIndices, GL_TRIANGLES);
	Renderable* CubeRenderable1 = renderer.addRenderable(cubeGeometryData, glm::translate(0.0f, 0.0f, 1.0f), shader);


	return app.exec();
	//int ret = app.exec();
	//renderer.shutdown();
	//return ret;
}