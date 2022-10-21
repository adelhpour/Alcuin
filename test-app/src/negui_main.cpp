#include "negui_c_api.h"
#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow mainWindow;
    mainWindow.setCentralWidget(MyNetworkEditorWidget_new(&mainWindow));
    mainWindow.show();
    return a.exec();
}

