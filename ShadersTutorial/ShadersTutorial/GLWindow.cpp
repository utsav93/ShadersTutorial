#include <gl\glew.h>
#include <GLWindow.h>
#include <glm.hpp>
#include <QtGui\qkeyevent>
#include <iostream>
#include <fstream>
#include <gtx\transform.hpp>
using namespace std;


void sendDataToOpenGL()

{
	//vertex data
	GLfloat verts[] =
	{
		+0.0f, +0.0f,
		+1.0f, +0.0f, +0.0f,
		+1.0f, +1.0f,
		+0.0f, +1.0f, +0.0f,
		-1.0f, +1.0f,
		+0.0f, +0.0f, +1.0f,
	};


	//array buffer setup
	GLuint  vertexMyBufferID;
	glGenBuffers(1, &vertexMyBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexMyBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts),
		verts, GL_STATIC_DRAW);

	//position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);

	//color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char*)(sizeof(float) *2 ));

	//element array buffer setup
	GLushort indices[] = { 0,1,2 };
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
		indices, GL_STATIC_DRAW);
}

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

float rotationX = 45.0f;
float rotationY = 0.0f;
float rotationZ = 0.0f;
float translateX = 0.5f;
float translateY = 0.5f;
float translateZ = 0.0f;
float scaleX = 0.1f;
float scaleY = 0.2f;
float scaleZ = 1.0f;
float translateChange = 0.1f;
float rotationChange = 5.0f;

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


void installShaders()

{
	
	GLuint programID = glCreateProgram();
	GLenum err = GL_NO_ERROR;
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
	
	

	glm::mat3 scaleMatrix = glm::mat3(glm::scale(scaleX, scaleY, scaleZ));

	glm::mat3 rotateMatrix = glm::mat3(glm::rotate(rotationX, rotationY, rotationZ, 1.0f));
	glm::mat3 translateMatrix;

	translateMatrix[2][0] = translateX;
	translateMatrix[2][1] = translateY;

	glm::mat3 transform = translateMatrix  * scaleMatrix * rotateMatrix;

	GLint scaleUniformLocation = glGetUniformLocation(programID, "transform");
	glUniformMatrix3fv(scaleUniformLocation, 1, GL_FALSE, &transform[0][0]);

	
	
}

void GLWindow::keyPressEvent(QKeyEvent* e)
{
	{
		switch (e->key())
		{
		case Qt::Key::Key_W:
			translateY += translateChange;
			break;
		case Qt::Key::Key_S:
			translateY -= translateChange;
			break;
		case Qt::Key::Key_A:
			translateX -= translateChange;
			break;
		case Qt::Key::Key_D:
			translateX += translateChange;
			break;
		case Qt::Key::Key_Q:
			rotationX += rotationChange;
			break;
		case Qt::Key::Key_E:
			rotationX -= rotationChange;
			break;
		}
		installShaders();
		paintGL();
	}
 }


void GLWindow::initializeGL()
{
	glewInit();
	sendDataToOpenGL();
	installShaders();
	
}

void GLWindow::paintGL()
{
	//window
	glViewport(0, 0, width(), height());

	//glDrawArrays(GL_TRIANGLES, 0, 6); draw with just array buffer

	//draw with element array
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}