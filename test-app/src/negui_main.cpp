#include "negui_c_api.h"
#include "negui_main_window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow* mainWindow = new MyMainWindow();
    mainWindow->show();
    return a.exec();
}

