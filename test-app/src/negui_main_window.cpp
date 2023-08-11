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
    connect(menuBar, SIGNAL(askForListOfPluginItemCategories(const QString&)), centralWidget(), SIGNAL(askForListOfPluginItemCategories(const QString&)));
    connect(menuBar, SIGNAL(askForReadFromFile(const QString&)), centralWidget(), SIGNAL(askForReadFromFile(const QString&)));
    connect(menuBar, SIGNAL(askForSaveCurrentNetwork()), centralWidget(), SIGNAL(askForSaveCurrentNetwork()));
    connect(menuBar, SIGNAL(askForWriteDataToFile(const QString&)), centralWidget(), SIGNAL(askForWriteDataToFile(const QString&)));
    connect(menuBar, SIGNAL(askForWriteFigureToFile(const QString&)), centralWidget(), SIGNAL(askForWriteFigureToFile(const QString&)));
    connect((MyMenuBar*)menuBar, &MyMenuBar::askForCloseTheApp, centralWidget(), [this] () { close(); });
    connect(menuBar, SIGNAL(askForTriggerUndoAction()), centralWidget(), SIGNAL(askForTriggerUndoAction()));
    connect(centralWidget(), SIGNAL(canUndoChanged(const bool&)), menuBar, SIGNAL(canUndoChanged(const bool&)));
    connect(menuBar, SIGNAL(askForTriggerRedoAction()), centralWidget(), SIGNAL(askForTriggerRedoAction()));
    connect(centralWidget(), SIGNAL(canRedoChanged(const bool&)), menuBar, SIGNAL(canRedoChanged(const bool&)));
    connect(menuBar, SIGNAL(askForCutSelectedNetworkElements()), centralWidget(), SIGNAL(askForCutSelectedNetworkElements()));
    connect(centralWidget(), SIGNAL(elementsCuttableStatusChanged(const bool&)), menuBar, SIGNAL(elementsCuttableStatusChanged(const bool&)));
    connect(menuBar, SIGNAL(askForCopySelectedNetworkElements()), centralWidget(), SIGNAL(askForCopySelectedNetworkElements()));
    connect(centralWidget(), SIGNAL(elementsCopyableStatusChanged(const bool&)), menuBar, SIGNAL(elementsCopyableStatusChanged(const bool&)));
    connect(menuBar, SIGNAL(askForPasteCopiedNetworkElements()), centralWidget(), SIGNAL(askForPasteCopiedNetworkElements()));
    connect(centralWidget(), SIGNAL(pasteElementsStatusChanged(const bool&)), menuBar, SIGNAL(pasteElementsStatusChanged(const bool&)));
    connect(menuBar, SIGNAL(askForSelectAllElements()), centralWidget(), SIGNAL(askForSelectAllElements()));
    connect(menuBar, SIGNAL(askForSelectAllElements(const QString&)), centralWidget(), SIGNAL(askForSelectAllElements(const QString&)));
    connect((MyMenuBar*)menuBar, &MyMenuBar::askForName, this, [this] () { return QString(std::string(getName()).c_str()); });
    connect((MyMenuBar*)menuBar, &MyMenuBar::askForVersionNumber, this, [this] () { return QString(std::string(getVersionNumber()).c_str()); });
}