#include <Qt\QApplication.h>
#include <Renderer.h>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	//renderer.initialize();
	renderer.show();
	int ret = app.exec();
	//renderer.shutdown();
	return ret;
}