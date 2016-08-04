#include <gl\glew.h>
#include <GLWindow.h>


void GLWindow::initializeGL()
{
	glewInit();
	
	//vertex data
	GLfloat verts[] =
	{
		+0.0f, +0.0f,
		+1.0f, +0.0f, +0.0f,
		+1.0f, +1.0f,
		+1.0f, +0.0f, +0.0f,
		-1.0f, +1.0f,
		+1.0f, +0.0f, +0.0f,
		-1.0f, -1.0f,
		+1.0f, +0.0f, +0.0f,
		+1.0f, -1.0f
		+1.0f, +0.0f, +0.0f,
	};

	//array buffer setup
	GLuint  vertexmyBufferID;
	glGenBuffers(1, &vertexmyBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexmyBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts),
		verts, GL_STATIC_DRAW);

	//position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);

	//color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char*)(sizeof(float) * 2));

	//element array buffer setup
	GLushort indices[] = { 0,1,2, 0,3,4 };
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
		indices, GL_STATIC_DRAW);

}

void GLWindow::paintGL()
{
	//window
	glViewport(0, 0, width(), height());

	//glDrawArrays(GL_TRIANGLES, 0, 6); draw with just array buffer

	//draw with element array
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}