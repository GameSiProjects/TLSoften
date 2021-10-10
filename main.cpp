#include "mainwindow.h"

#include <QApplication>
#include <QStyle>

int main(int argc, char *argv[])
{
	QApplication::setStyle("fusion");
	QApplication app(argc, argv);
	app.setWindowIcon(app.style()->standardIcon(QStyle::SP_DialogOkButton));
    MainWindow mainWindow;
    mainWindow.resize(800, 400);
    mainWindow.show();
    return app.exec();
}
