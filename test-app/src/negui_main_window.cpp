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
    QMenuBar* menuBar = new MyMenuBar();
    QMainWindow::setMenuBar(menuBar);
    connect(menuBar, SIGNAL(askForSetNewNetworkCanvas()), centralWidget(), SIGNAL(askForSetNewNetworkCanvas()));
    connect(menuBar, SIGNAL(askForSelectElements()), centralWidget(), SIGNAL(askForSelectElements()));
}