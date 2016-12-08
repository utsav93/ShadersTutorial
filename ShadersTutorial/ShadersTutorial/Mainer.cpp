#include <Qt\QApplication.h>
#include <gtx\transform.hpp>
#include <Renderer.h>
#include <ShapeGenerator.h>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	ShapeData data = ShapeGenerator::makeTriangle();

	renderer.show();


	Geometry* triGeometryData = renderer.addGeometry(data.vertices, data.vertexBufferSize(), data.indices, data.numIndices, GL_TRIANGLES);
	ShaderProgramInfo* shader = renderer.addShaderProgram("vertexShaderCode.glsl", "FragmentShaderCode.glsl");
	Renderable* triRenderable1 = renderer.addRenderable(triGeometryData, glm::translate(0.0f, 0.0f, 1.0f), shader);


	return app.exec();
	//int ret = app.exec();
	//renderer.shutdown();
	//return ret;
}