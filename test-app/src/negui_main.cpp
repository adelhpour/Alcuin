#include "negui_c_api.h"
#include "negui_main_window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyMainWindow mainWindow;
    mainWindow.setCentralWidget(createNetworkEditorWidget(&mainWindow));
    mainWindow.setGeometry(mainWindow.centralWidget()->geometry());
    mainWindow.show();
    return a.exec();
}

