#include <Qt\qapplication.h>
//#include <MeGlWindow.h>
#include<Renderer\MeRenderer.h>
//#include <Primitives\ShapeGenerator.h>
#include <Primitives\ShapeGenerator.h>
#include <glm\gtx\transform.hpp>

int main(int argc, char* argv[])
{

	QApplication app(argc, argv);

	ShapeData planeData = ShapeGenerator::makePlane();
	ShapeData cubeData = ShapeGenerator::makeCube();	
	ShapeData teapotData = ShapeGenerator::makeTeapot();
//	ShapeData sphereData = ShapeGenerator::makeSphere();
	ShapeData objData = ShapeGenerator::makeOgre();
	renderer.show();

	Geometry* planeGeometry =
		renderer.addGeometry(
			planeData.vertices, planeData.vertexBufferSize(),
			planeData.indices, planeData.numIndices, GL_TRIANGLES);
	Geometry* cubeGeometry = 
		renderer.addGeometry(
		cubeData.vertices, cubeData.vertexBufferSize(),
		cubeData.indices, cubeData.numIndices, GL_TRIANGLES);	
	Geometry* teapotGeometry =
		renderer.addGeometry(
			teapotData.vertices, teapotData.vertexBufferSize(),
			teapotData.indices, teapotData.numIndices, GL_TRIANGLES);
	Geometry* objGeometry =
		renderer.addGeometry(
			objData.vertices, objData.vertexBufferSize(),
			objData.indices, objData.numIndices, GL_TRIANGLES);

	ShaderProgramInfo* shader = renderer.addShaderProgram(
		"VertexShaderTest.glsl",
		"FragmentShaderTest.glsl");

	ShaderProgramInfo* shaderLight = renderer.addShaderProgram(
		"VertexShaderCodeNoLight.glsl",
		"FragmentShaderCodeNoLight.glsl"
	);

	Renderable* lightCube1 = renderer.addLight(cubeGeometry,
		glm::mat4(), shaderLight);

	Renderable* planeRenderable1 = renderer.addRenderable(planeGeometry,
		glm::translate(0.0f, -3.0f, -5.0f), shader
		);
	Renderable* cubeRenderable1 = renderer.addRenderable(cubeGeometry,
		glm::translate(-2.0f, -1.0f, -2.0f)*glm::scale(0.4f, 0.4f, 0.4f), shader);
//	Renderable* teapotRenderable1 = renderer.addRenderable(teapotGeometry,
//		glm::translate(1.0f, -1.5f, -2.0f)*glm::rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f))*glm::scale(0.1f, 0.1f, 0.1f), shader);
	Renderable* objRenderable1 = renderer.addRenderable(objGeometry,
		glm::translate(0.0f, 0.0f, -3.6f)*glm::scale(1.0f, 1.0f, 1.0f), shader, "unityChan.png"
		);
	return app.exec();
	system("PAUSE");
}