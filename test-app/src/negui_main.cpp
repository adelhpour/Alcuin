#include "negui_c_api.h"
#include "negui_menu_bar.h"

#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow* mainWindow = new QMainWindow();
    QWidget* networkEditorWidget = createNetworkEditorWidget(mainWindow);
    mainWindow->setCentralWidget(networkEditorWidget);
    mainWindow->setGeometry(networkEditorWidget->geometry());
    mainWindow->setMenuBar(new MyMenuBar());
    mainWindow->show();
    return a.exec();
}

