#include <gl\glew.h>
#include <GLWindow.h>
#include <glm.hpp>
#include <iostream>
#include <gtx\transform.hpp>
using namespace std;

extern const char* vertexShaderCode;
extern const char* fragmentShaderCode;

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
//if ((err = glGetError()) != GL_NO_ERROR)
//{
//	printf("%x%d", err, __LINE__);
//}
//checkGlProgram(programID, __FILE__, __LINE__);

/*glGetShaderInfoLog(vertexShaderID, 256, &returnSize, dataBuffer);
glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &dumbReturn);
GLchar dataBuffer[256];
GLsizei returnSize;
GLint dumbReturn;
if (dumbReturn == GL_TRUE)
{
	printf("Ok");
}

else if (dumbReturn == GL_FALSE)
{
	printf("Why?S");
}
else
{
	printf("kill me now");
}*/

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
	//points to character string(vertex) defined in shadercode.cpp
	adapter[0] = vertexShaderCode;
	glShaderSource(vertexShaderID, 1, adapter, 0);

	adapter[0] = fragmentShaderCode;
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;
	
	
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		cout << "error" << endl;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	

	glUseProgram(programID);

	GLint scaleUniformLocation = glGetUniformLocation(programID, "transform");

	glm::mat3 scaleMatrix = glm::mat3(glm::scale(0.1f, 0.2f, 1.0f));
	

	glm::mat3 translateMatrix;
	translateMatrix[2][2] = 0.5f;
	translateMatrix[2][1] = 0.5f;


	glm::mat3 transform = translateMatrix * scaleMatrix;


	glUniformMatrix3fv(scaleUniformLocation, 1, GL_FALSE, &transform[0][0]);
	
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