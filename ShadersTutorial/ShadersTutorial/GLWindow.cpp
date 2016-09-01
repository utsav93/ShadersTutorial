#include <gl\glew.h>
#include <GLWindow.h>

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

void installShaders()
{
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
	if ((err = glGetError()) != GL_NO_ERROR)
	{
		printf("%x%d", err, __LINE__);
	}
	glCompileShader(vertexShaderID);
	if ((err = glGetError()) != GL_NO_ERROR)
	{
		printf("%x%d", err, __LINE__);
	}
	glCompileShader(fragmentShaderID);
	if ((err = glGetError()) != GL_NO_ERROR)
	{
		printf("%x%d", err, __LINE__);
	}

	GLchar dataBuffer[256];
	GLsizei returnSize;
	GLint dumbReturn;

	glGetShaderInfoLog(vertexShaderID, 256, &returnSize, dataBuffer);
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &dumbReturn);

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
	}
	
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	if ((err = glGetError()) != GL_NO_ERROR)
	{
		printf("%x%d", err, __LINE__);
	}
	glAttachShader(programID, fragmentShaderID);
	if ((err = glGetError()) != GL_NO_ERROR)
	{
		printf("%x%d", err, __LINE__);
	}
	glLinkProgram(programID);
	checkGlProgram(programID, __FILE__, __LINE__);
	if ((err = glGetError()) != GL_NO_ERROR)
	{
		printf("%x%d", err, __LINE__);
	}
	glUseProgram(programID);
	
	if ((err = glGetError()) != GL_NO_ERROR)
	{
		printf("%x%d", err, __LINE__);
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
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}