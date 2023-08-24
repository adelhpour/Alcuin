#include "negui_main_window.h"

#include <QApplication>

int main()
{
    int argc = 3;
    char*argv[] = {(char*)"Appname", (char*)"--platform", (char*)"windows:dpiawareness=0"};
    QApplication a(argc, argv);
    QMainWindow* mainWindow = new MyMainWindow();
    mainWindow->show();
    return a.exec();
}

