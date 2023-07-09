#include "negui_c_api.h"
#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow mainWindow;
    mainWindow.setCentralWidget(createNetworkEditorWidget(&mainWindow));
    mainWindow.setGeometry(mainWindow.centralWidget()->geometry());
    mainWindow.show();
    return a.exec();
}

