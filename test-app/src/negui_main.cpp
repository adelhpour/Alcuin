#include "negui_main_window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    QMainWindow* mainWindow = new MyMainWindow();
    mainWindow->show();
    return application.exec();
}

