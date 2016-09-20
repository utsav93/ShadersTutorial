#ifndef GL_WINDOW
#define GL_WINDOW
#include <QtOpenGL\qglwidget>
#include <QT\QTimer.h>

class GLWindow : public QGLWidget
{
protected:
	void initializeGL();
	void paintGL();
	QTimer *timer;
public:
};

#endif