#include "negui_main_window.h"
#include "negui_c_api.h"
#include "negui_menu_bar.h"


MyMainWindow::MyMainWindow(QWidget* parent) : QMainWindow(parent) {
    QWidget* networkEditorWidget = createNetworkEditorWidget(this);
    setCentralWidget(networkEditorWidget);
    setGeometry(networkEditorWidget->geometry());
    setMenuBar();
}

void MyMainWindow::setMenuBar() {
    MyMenuBar* menuBar = new MyMenuBar();
    connectToCentralWidget(menuBar);
    menuBar->setMenus();
    QMainWindow::setMenuBar(menuBar);
}

void MyMainWindow::connectToCentralWidget(QWidget* menuBar) {
    connect(menuBar, SIGNAL(askForSetNewNetworkCanvas()), centralWidget(), SIGNAL(askForSetNewNetworkCanvas()));
    connect(menuBar, SIGNAL(askForListOfPluginItemNames(const QString&)), centralWidget(), SIGNAL(askForListOfPluginItemNames(const QString&)));
    connect(menuBar, SIGNAL(askForReadFromFile(const QString&)), centralWidget(), SIGNAL(askForReadFromFile(const QString&)));
    connect(menuBar, SIGNAL(askForSaveCurrentNetwork()), centralWidget(), SIGNAL(askForSaveCurrentNetwork()));
    connect(menuBar, SIGNAL(askForWriteDataToFile(const QString&)), centralWidget(), SIGNAL(askForWriteDataToFile(const QString&)));
    connect(menuBar, SIGNAL(askForSelectElements()), centralWidget(), SIGNAL(askForSelectElements()));
}