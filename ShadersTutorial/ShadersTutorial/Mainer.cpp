#include <Qt\QApplication.h>
#include <GLWindow.h>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	GLWindow myWindow;
	myWindow.show();

	return app.exec();
}