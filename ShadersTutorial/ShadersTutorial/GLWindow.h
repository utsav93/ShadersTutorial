#ifndef GL_WINDOW
#define GL_WINDOW
#include <QtOpenGL\qglwidget>
#include <QT\QTimer.h>

class GLWindow : public QGLWidget
{
protected:
	void initializeGL();
	void paintGL();
	void sendDataToOpenGL();
	void installShaders();
	bool checkProgramStatus(GLuint);
	bool checkShaderStatus(GLuint);
	void checkGlProgram(GLuint, const char*, int);
	QTimer *timer;
public:
	~GLWindow();
};

#endif