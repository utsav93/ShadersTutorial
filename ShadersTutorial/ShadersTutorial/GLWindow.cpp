#include <gl\glew.h>
#include <GLWindow.h>
#include <glm.hpp>
#include <QtGui\qkeyevent>
#include <iostream>
#include <fstream>
#include <gtx\transform.hpp>
#include <Vertex.h>
#include <ShapeGenerator.h>


using namespace std;

GLuint programID;
GLuint numIndices;

float cubeRotationX = 0.0f;
float rotationChange = 2.0f;

void sendDataToOpenGL()

{
	ShapeData shape = ShapeGenerator::makeCube();

	//array buffer setup
	GLuint  vertexMyBufferID;
	glGenBuffers(1, &vertexMyBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexMyBufferID);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);

	//position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	//color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) *3 ));

	//element array buffer setup
	
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);
	numIndices = shape.numIndices;
	shape.cleanup();
}

//error checking code
void checkGlProgram(GLuint prog, const char *file, int line)
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

//shader compile check
bool checkShaderStatus(GLuint shaderID)
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
bool checkProgramStatus(GLuint programID)
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
string readShaderCode(const char* fileName)
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
void installShaders()

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
	glewInit();
	timer = new QTimer;
	glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	installShaders();
	
}

GLWindow::~GLWindow()
{
	glUseProgram(0);
	glDeleteProgram(programID);
}

void GLWindow::paintGL()
{
	//window
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	GLint fullTransformMatrixUniformLocation;
	glm::mat4 fullTransformMatrix;

	cubeRotationX += rotationChange;
	if (cubeRotationX >= 360.0f)
	{
		cubeRotationX = 0.0f;
	}
	glm::mat4 projectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 10.0f);

	//Cube1
	glm::mat4 translationMatrix = glm::translate(glm::mat4(), glm::vec3(-1.0f, 0.0f, -3.0f));
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), 54.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	

	fullTransformMatrix = projectionMatrix * translationMatrix * rotationMatrix;

	fullTransformMatrixUniformLocation = glGetUniformLocation(programID, "fullTransformMatrix");

	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);

	//glDrawArrays(GL_TRIANGLES, 0, 6); draw with just array buffer

	//draw with element array
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);

	//Cube2
	translationMatrix = glm::translate(glm::mat4(), glm::vec3(1.0f, 0.0f, -3.75f));
	rotationMatrix = glm::rotate(glm::mat4(), 126.0f, glm::vec3(0.0f, 1.0f, 0.0f));


	fullTransformMatrix = projectionMatrix * translationMatrix * rotationMatrix;

	fullTransformMatrixUniformLocation = glGetUniformLocation(programID, "fullTransformMatrix");

	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);

	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->setInterval(16);
	timer->start();

}