#include <gl\glew.h>
#include <GLWindow.h>
#include <glm.hpp>
#include <QtGui\qkeyevent>
#include <iostream>
#include <fstream>
#include <gtx\transform.hpp>
#include <Vertex.h>
#include <ShapeGenerator.h>
#include <Camera.h>
#include <QTGui\Qmouseevent>
#include <QtGui\Qkeyevent>


using namespace std;

GLuint programID;
GLuint planeNumIndices;
GLuint arrowNumIndices;
GLuint cubeNumIndices;
GLuint teapotNumIndices;
GLuint sphereNumIndices;
GLuint torusNumIndices;

GLuint modelToProjectionUniformLocation;
GLint lightPositionWorldUniformLocation;
GLint ambientLightUniformLocation;
GLint modelToWorldMatrixUniformLocation;
GLint cameraPositionUniformLocation;

GLuint theBufferID;

glm::vec3 diffuseLightPosition(0.0f, 0.5f, 0.0f);
GLfloat lightPositionWorldChange = 0.2f;

GLfloat cubeRotation = 0.0f;
GLfloat planeRotation = 0.0f;
GLfloat arrowRotation = 0.0f;
GLfloat teapotRotation = 0.0f;
GLfloat sphereRotation = 0.0f;
GLfloat torusRotation = 0.0f;
GLfloat rotationChange = 2.0f;

GLuint planeVertexArrayObjectID;
GLuint arrowVertexArrayObjectID;
GLuint cubeVertexArrayObjectID;
GLuint teapotVertexArrayObjectID;
GLuint sphereVertexArrayObjectID;
GLuint torusVertexArrayObjectID;

GLuint planeIndexByteOffset;
GLuint arrowIndexByteOffset;
GLuint cubeIndexByteOffset;
GLuint teapotIndexByteOffset;
GLuint sphereIndexByteOffset;
GLuint torusIndexByteOffset;

GLuint arrowByteOffset;
GLuint cubeByteOffset;
GLuint teapotByteOffset;
GLuint sphereByteOffset;
GLuint torusByteOffset;


Camera camera;

void GLWindow::sendDataToOpenGL()
{
	ShapeData plane = ShapeGenerator::makePlane();
	ShapeData arrow = ShapeGenerator::makeArrow();
	ShapeData cube = ShapeGenerator::makeCube();
	ShapeData teapot = ShapeGenerator::makeTeapot();
	ShapeData sphere = ShapeGenerator::makeSphere();
	ShapeData torus = ShapeGenerator::makeTorus();

	glGenBuffers(1, &theBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glBufferData(GL_ARRAY_BUFFER,
		plane.vertexBufferSize() + plane.indexBufferSize() +
		arrow.vertexBufferSize() + arrow.indexBufferSize() + 
		cube.vertexBufferSize() + cube.indexBufferSize() +
		teapot.vertexBufferSize() + teapot.indexBufferSize() +
		sphere.vertexBufferSize() + sphere.indexBufferSize() +
		torus.vertexBufferSize() + torus.indexBufferSize()
		, 0, GL_STATIC_DRAW);
	GLsizeiptr currentOffset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.vertexBufferSize(), plane.vertices);
	currentOffset += plane.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.indexBufferSize(), plane.indices);
	currentOffset += plane.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.vertexBufferSize(), arrow.vertices);
	currentOffset += arrow.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.indexBufferSize(), arrow.indices);
	currentOffset += arrow.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.vertexBufferSize(), cube.vertices);
	currentOffset += cube.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.indexBufferSize(), cube.indices);
	currentOffset += cube.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, teapot.vertexBufferSize(), teapot.vertices);
	currentOffset += teapot.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, teapot.indexBufferSize(), teapot.indices);
	currentOffset += teapot.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, sphere.vertexBufferSize(), sphere.vertices);
	currentOffset += sphere.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, sphere.indexBufferSize(), sphere.indices);
	currentOffset += sphere.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, torus.vertexBufferSize(), torus.vertices);
	currentOffset += torus.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, torus.indexBufferSize(), teapot.indices);
	currentOffset += torus.indexBufferSize();

	planeNumIndices = plane.numIndices;
	arrowNumIndices = arrow.numIndices;
	cubeNumIndices = cube.numIndices;
	teapotNumIndices = teapot.numIndices;
	sphereNumIndices = sphere.numIndices;
	torusNumIndices = torus.numIndices;

	glGenVertexArrays(1, &planeVertexArrayObjectID);
	glGenVertexArrays(1, &arrowVertexArrayObjectID);
	glGenVertexArrays(1, &cubeVertexArrayObjectID);
	glGenVertexArrays(1, &teapotVertexArrayObjectID);
	glGenVertexArrays(1, &sphereVertexArrayObjectID);
	glGenVertexArrays(1, &torusVertexArrayObjectID);

	//plane
	glBindVertexArray(planeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	//arrow
	arrowByteOffset = plane.vertexBufferSize() + plane.indexBufferSize();
	glBindVertexArray(arrowVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)arrowByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(arrowByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(arrowByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	//cube
	cubeByteOffset = arrowByteOffset + arrow.vertexBufferSize() + arrow.indexBufferSize();
	glBindVertexArray(cubeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)cubeByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(cubeByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(cubeByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	//teapot
	teapotByteOffset = cubeByteOffset + cube.vertexBufferSize() + cube.indexBufferSize();
	glBindVertexArray(teapotVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)teapotByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(teapotByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(teapotByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	//sphere
	sphereByteOffset = teapotByteOffset + teapot.vertexBufferSize() + teapot.indexBufferSize();
	glBindVertexArray(sphereVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)sphereByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sphereByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sphereByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	//torus
	torusByteOffset = sphereByteOffset + sphere.vertexBufferSize() + sphere.indexBufferSize();
	glBindVertexArray(torusVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)torusByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(torusByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(torusByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);


	planeIndexByteOffset = plane.vertexBufferSize();
	arrowIndexByteOffset = arrowByteOffset + arrow.vertexBufferSize();
	cubeIndexByteOffset = cubeByteOffset + cube.vertexBufferSize();
	teapotIndexByteOffset = teapotByteOffset + teapot.vertexBufferSize();
	sphereIndexByteOffset = sphereByteOffset + sphere.vertexBufferSize();
	torusIndexByteOffset = torusByteOffset + torus.vertexBufferSize();


	plane.cleanup();
	arrow.cleanup();
	cube.cleanup();
	teapot.cleanup();
	sphere.cleanup();
	torus.cleanup();
}
void GLWindow::paintGL()
{
	
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	glm::mat4 modelToProjectionMatrix;
	glm::mat4 viewToProjectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 20.0f);
	glm::mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	glm::mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

	glm::vec3 ambientLight(0.0f, 0.0f, 0.4f);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);
	lightPositionWorldUniformLocation = glGetUniformLocation(programID, "lightPositionWorld");
	glm::vec3 lightPositionWorld(diffuseLightPosition);
	glUniform3fv(lightPositionWorldUniformLocation, 1, &lightPositionWorld[0]);



	// Arrow
	//arrowModelToWorldMatrix = glm::mat4();
	glBindVertexArray(arrowVertexArrayObjectID);
	glm::mat4 arrowModelToWorldMatrix = glm::translate(0.0f, 1.0f, -3.0f) * glm::rotate(arrowRotation, 0.0f, 1.0f, 0.0f);
	modelToProjectionMatrix = worldToProjectionMatrix * arrowModelToWorldMatrix;
	glUniformMatrix4fv(modelToProjectionUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, &arrowModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, arrowNumIndices, GL_UNSIGNED_SHORT, (void*)arrowIndexByteOffset);

	arrowRotation += rotationChange;

	// Plane
	glBindVertexArray(planeVertexArrayObjectID);
	glm::mat4 planeModelToWorldMatrix = glm::rotate(planeRotation, 1.0f, 0.0f, 0.0f);
	modelToProjectionMatrix = worldToProjectionMatrix * planeModelToWorldMatrix;
	glUniformMatrix4fv(modelToProjectionUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, &planeModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, planeNumIndices, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffset);

	//planeRotation += rotationChange;

	//cube1
	glBindVertexArray(cubeVertexArrayObjectID);
	glm::mat4 cube1ModelToWorldMatrix = glm::translate(diffuseLightPosition) * glm::scale(0.1f, 0.1f, 0.1f);
	modelToProjectionMatrix = worldToProjectionMatrix * cube1ModelToWorldMatrix;
	glUniformMatrix4fv(modelToProjectionUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, &cube1ModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);

	cubeRotation += rotationChange;
	//cube2
	glBindVertexArray(cubeVertexArrayObjectID);
	glm::mat4 cube2ModelToWorldMatrix = glm::translate(1.0f, 2.0f, -2.0f) * glm::scale(0.4f, 0.4f, 0.4f) * glm::rotate(cubeRotation, 1.0f, 0.0f, 0.0f);
	modelToProjectionMatrix = worldToProjectionMatrix * cube2ModelToWorldMatrix;
	glUniformMatrix4fv(modelToProjectionUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, &cube2ModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);

	// Teapot
	glBindVertexArray(teapotVertexArrayObjectID);
	glm::mat4 teapotModelToWorldMatrix =
		glm::translate(3.0f, 0.0f, 0.0f) *
		glm::rotate(-90.0f, 0.0f, 1.0f, 0.0f) *
		glm::rotate(-90.0f, 1.0f, 0.0f, 0.0f) *
		glm::scale(0.5f, 0.5f, 0.5f);
	modelToProjectionMatrix = worldToProjectionMatrix * teapotModelToWorldMatrix;
	glUniformMatrix4fv(modelToProjectionUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, &teapotModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, teapotNumIndices, GL_UNSIGNED_SHORT, (void*)teapotIndexByteOffset);

	// Sphere
	glBindVertexArray(sphereVertexArrayObjectID);
	glm::mat4 sphereModelToWorldMatrix = glm::translate(3.0f, 3.0f, 3.0f);
	modelToProjectionMatrix = worldToProjectionMatrix * sphereModelToWorldMatrix;
	glUniformMatrix4fv(modelToProjectionUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, &sphereModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, sphereNumIndices, GL_UNSIGNED_SHORT, (void*)sphereIndexByteOffset);

	// Torus
	glBindVertexArray(torusVertexArrayObjectID);
	glm::mat4 torusModelToWorldMatrix = glm::translate(0.0f, 2.0f, 0.0f);
	modelToProjectionMatrix = worldToProjectionMatrix * torusModelToWorldMatrix;
	glUniformMatrix4fv(modelToProjectionUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, &torusModelToWorldMatrix[0][0]);
	//glDrawElements(GL_TRIANGLES, torusNumIndices, GL_UNSIGNED_SHORT, (void*)torusIndexByteOffset);

	glm::vec3 cameraPositionWorld = camera.getPosition();
	glUniform3f(cameraPositionUniformLocation, cameraPositionWorld.x, cameraPositionWorld.y, cameraPositionWorld.z);

	connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
	timer.setInterval(16);
	timer.start();
	

}

//error checking code
void GLWindow::checkGlProgram(GLuint prog, const char *file, int line)
{
	GLint status;
	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		int loglen;
		char logbuffer[1000];
		glGetProgramInfoLog(prog, sizeof(logbuffer), &loglen, logbuffer);
		fprintf(stderr, "OpenGL Program Linker Error at %s:%d:\n%.*s", file, line, loglen, logbuffer);
	}
	else {
		int loglen;
		char logbuffer[1000];
		glGetProgramInfoLog(prog, sizeof(logbuffer), &loglen, logbuffer);
		if (loglen > 0) {
			fprintf(stderr, "OpenGL Program Link OK at %s:%d:\n%.*s", file, line, loglen, logbuffer);
		}
		glValidateProgram(prog);
		glGetProgramInfoLog(prog, sizeof(logbuffer), &loglen, logbuffer);
		if (loglen > 0) {
			fprintf(stderr, "OpenGL Program Validation results at %s:%d:\n%.*s", file, line, loglen, logbuffer);
		}
	}
}

void GLWindow::mouseMoveEvent(QMouseEvent* e)
{
	camera.mouseUpdate(glm::vec2(e->x(), e->y()));
	repaint();
}

void GLWindow::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key::Key_W:
		camera.moveForward();
		break;
	case Qt::Key::Key_A:
		camera.strafeLeft();
		break;
	case Qt::Key::Key_S:
		camera.moveBackward();
		break;
	case Qt::Key::Key_D:
		camera.strafeRight();
		break;
	case Qt::Key::Key_R:
		camera.moveUp();
		break;
	case Qt::Key::Key_F:
		camera.moveDown();
		break;
	case Qt::Key::Key_Left:
		diffuseLightPosition.x -= lightPositionWorldChange;
		break;
	case Qt::Key::Key_Right:
		diffuseLightPosition.x += lightPositionWorldChange;
		break;
	case Qt::Key::Key_Up:
		diffuseLightPosition.y += lightPositionWorldChange;
		break;
	case Qt::Key::Key_Down:
		diffuseLightPosition.y -= lightPositionWorldChange;
		break;
	case Qt::Key::Key_Z:
		diffuseLightPosition.z += lightPositionWorldChange;
		break;
	case Qt::Key::Key_X:
		diffuseLightPosition.z -= lightPositionWorldChange;
		break;
	}
	repaint();
}

//shader compile check
bool GLWindow::checkShaderStatus(GLuint shaderID)
{
	GLint compileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		GLint infoLogLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];
		GLsizei bufferSize;
		glGetShaderInfoLog(shaderID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
}

//linker error check
bool GLWindow::checkProgramStatus(GLuint programID)
{
	GLint linkStatus;
	glGetProgramiv(programID, GL_COMPILE_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE)
	{
		GLint infoLogLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];
		GLsizei bufferSize;
		glGetProgramInfoLog(programID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
}


//read shader code from files function
static string readShaderCode(const char* fileName)
{
	ifstream myInput(fileName);
	if (!myInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(myInput),
		std::istreambuf_iterator<char>());
}


//compiling and calling shaders 
void GLWindow::installShaders()

{
	
	programID = glCreateProgram();
	//Vertex Shader Object
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

	//Fragment Shader Object
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	
	//array of pointers
	const char* adapter[1];
	string temp = readShaderCode("VertexShaderCode.glsl");
	//points to character string(vertex) defined in shadercode.cpp
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);
	
	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);
	
	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		cout << "error compiling shaders" << endl;
	
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);

	checkGlProgram(programID, __FILE__, __LINE__);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);


	glUseProgram(programID);
	checkGlProgram(programID, __FILE__, __LINE__);

	
	
}

void GLWindow::initializeGL()
{
	setMouseTracking(true);
	glewInit();
	glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	installShaders();
	glEnable(GL_CULL_FACE);
	modelToWorldMatrixUniformLocation = glGetUniformLocation(programID, "modelToWorldMatrix");
	modelToProjectionUniformLocation = glGetUniformLocation(programID, "modelToProjectionMatrix");
	lightPositionWorldUniformLocation = glGetUniformLocation(programID, "lightPositionWorld");
	ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");
	cameraPositionUniformLocation = glGetUniformLocation(programID, "cameraPositionWorld");
	
}

GLWindow::~GLWindow()
{
	glDeleteBuffers(1, &theBufferID);
	glUseProgram(0);
	glDeleteProgram(programID);
}

