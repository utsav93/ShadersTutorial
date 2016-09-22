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
GLuint cubeNumIndices;
GLuint arrowNumIndices;
GLuint fullTransformUniformLocation;

GLuint theBufferID;

GLuint cubeVertexArrayObjectID;
GLuint arrowVertexArrayObjectID;
GLuint cubeIndexByteOffset;
GLuint arrowIndexByteOffset;

float cubeRotationX = 0.0f;
float rotationChange = 2.0f;
Camera camera;

void GLWindow::sendDataToOpenGL()
{
	ShapeData cube = ShapeGenerator::makePlane();
	ShapeData arrow = ShapeGenerator::makeArrow();

	glGenBuffers(1, &theBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glBufferData(GL_ARRAY_BUFFER,
		cube.vertexBufferSize() + cube.indexBufferSize() +
		arrow.vertexBufferSize() + arrow.indexBufferSize(), 0, GL_STATIC_DRAW);
	GLsizeiptr currentOffset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.vertexBufferSize(), cube.vertices);
	currentOffset += cube.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.indexBufferSize(), cube.indices);
	currentOffset += cube.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.vertexBufferSize(), arrow.vertices);
	currentOffset += arrow.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.indexBufferSize(), arrow.indices);

	cubeNumIndices = cube.numIndices;
	arrowNumIndices = arrow.numIndices;

	glGenVertexArrays(1, &cubeVertexArrayObjectID);
	glGenVertexArrays(1, &arrowVertexArrayObjectID);

	glBindVertexArray(cubeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sizeof(float) * 3));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	glBindVertexArray(arrowVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint arrowByteOffset = cube.vertexBufferSize() + cube.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)arrowByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(arrowByteOffset + sizeof(float) * 3));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	cubeIndexByteOffset = cube.vertexBufferSize();
	arrowIndexByteOffset = arrowByteOffset + arrow.vertexBufferSize();

	cube.cleanup();
	arrow.cleanup();
}
void GLWindow::paintGL()
{

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	glm::mat4 fullTransformMatrix;
	glm::mat4 viewToProjectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 20.0f);
	glm::mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	glm::mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");
	glm::vec3 ambientLight(0.5f, 0.5f, 0.5f);
	glUniform3f(ambientLightUniformLocation, ambientLight.x, ambientLight.y, ambientLight.z);

	// Cube
	glBindVertexArray(cubeVertexArrayObjectID);
	glm::mat4 cube1ModelToWorldMatrix = glm::translate(glm::vec3(1.0f, 0.0f, -3.0f)) * glm::rotate(0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	fullTransformMatrix = worldToProjectionMatrix * cube1ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);

	glm::mat4 cube2ModelToWorldMatrix = glm::translate(glm::vec3(2.0f, 0.0f, -3.75f)) * glm::rotate(126.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	fullTransformMatrix = worldToProjectionMatrix * cube2ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);

	// Arrow
	glBindVertexArray(arrowVertexArrayObjectID);
	glm::mat4 arrowModelToWorldMatrix = glm::translate(0.0f, 1.5f, -3.0f);
	fullTransformMatrix = worldToProjectionMatrix * arrowModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, arrowNumIndices, GL_UNSIGNED_SHORT, (void*)arrowIndexByteOffset);

	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->setInterval(16);
	timer->start();

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
	timer = new QTimer;
	glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	installShaders();
	fullTransformUniformLocation = glGetUniformLocation(programID, "fullTransformMatrix");
	
}

GLWindow::~GLWindow()
{
	glDeleteBuffers(1, &theBufferID);
	glUseProgram(0);
	glDeleteProgram(programID);
}

