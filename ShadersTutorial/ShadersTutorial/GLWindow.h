#ifndef GL_WINDOW
#define GL_WINDOW
#include <QtOpenGL\qglwidget>

class GLWindow : public QGLWidget
{
protected:
	void initializeGL();
	void paintGL();
public:
};

#endif