#include <Qt\QApplication.h>
#include <Renderer.h>
#include <ShapeGenerator.h>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	//renderer.initialize();
	ShapeData data = ShapeGenerator::makeCube();
	Geometry* cubeGeometryData = renderer.addGeometry(data.vertices, data.vertexBufferSize(), data.indices, data.numIndices, GL_TRIANGLES);
	Renderable* CubeRenderable1 = renderer.addRenderable(cubeGeometryData);
	renderer.show();
	return app.exec();
	//int ret = app.exec();
	//renderer.shutdown();
	//return ret;
}