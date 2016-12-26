#include <QtWidgets/QApplication>
 
#include "Shadow.h"
#include "Device.h"

int main(int argc, char *argv[])
{
	
	QApplication a(argc, argv);
	
	sh::CreationParameters params;
	params.driverType = sh::video::DriverType::OPENGL_ES_2_0;
	params.width = 800;
	params.height = 600;

	sh::Device* device = sh::CreateDevice(params);
	
	return a.exec();
}