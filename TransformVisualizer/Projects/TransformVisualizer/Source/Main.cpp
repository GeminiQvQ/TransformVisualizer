// Project Includes
#include <TransformVisualizer/Kernel.h>

// Third-Party Includes
#include <QApplication>

// Platform Includes
#include <windows.h>

int run(QApplication& qapp)
{
	TransformVisualizer::Kernel kernel;
	kernel.create();

	int result = qapp.exec();

	kernel.destroy();

	return result;
}

int main(int argc, char** argv)
{
	QApplication qapp(argc, argv);
	return run(qapp);
}

int WINAPI WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
	int argc = 0;
	QApplication qapp(argc, nullptr);
	return run(qapp);
}