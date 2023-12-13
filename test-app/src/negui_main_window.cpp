#include "negui_main_window.h"
#include "negui_menu_bar.h"
#include "negui_main_widget_builder.h"


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
    connect(menuBar, SIGNAL(askForSetNewNetworkCanvas()), centralWidget(), SLOT(setNewNetworkCanvas()));
    connect(menuBar, SIGNAL(askForListOfPluginItemNames(const QString&)), centralWidget(), SLOT(listOfPluginItemNames(const QString&)));
    connect(menuBar, SIGNAL(askForListOfPluginItemCategories(const QString&)), centralWidget(), SLOT(listOfPluginItemCategories(const QString&)));
    connect(menuBar, SIGNAL(askForDefaultPluginAction(const QString&)), centralWidget(), SLOT(defaultPluginAction(const QString&)));
    connect(menuBar, SIGNAL(askForSaveCurrentNetwork()), centralWidget(), SLOT(saveCurrentNetwork()));
    connect((MyMenuBar*)menuBar, &MyMenuBar::askForCloseTheApp, centralWidget(), [this] () { close(); });
    connect(menuBar, SIGNAL(askForTriggerUndoAction()), centralWidget(), SLOT(triggerUndoAction()));
    connect(centralWidget(), SIGNAL(canUndoChanged(const bool&)), menuBar, SIGNAL(canUndoChanged(const bool&)));
    connect(menuBar, SIGNAL(askForTriggerRedoAction()), centralWidget(), SLOT(triggerRedoAction()));
    connect(centralWidget(), SIGNAL(canRedoChanged(const bool&)), menuBar, SIGNAL(canRedoChanged(const bool&)));
    connect(menuBar, SIGNAL(askForCutSelectedNetworkElements()), centralWidget(), SLOT(cutSelectedNetworkElements()));
    connect(centralWidget(), SIGNAL(elementsCuttableStatusChanged(const bool&)), menuBar, SIGNAL(elementsCuttableStatusChanged(const bool&)));
    connect(menuBar, SIGNAL(askForCopySelectedNetworkElements()), centralWidget(), SLOT(copySelectedNetworkElements()));
    connect(centralWidget(), SIGNAL(elementsCopyableStatusChanged(const bool&)), menuBar, SIGNAL(elementsCopyableStatusChanged(const bool&)));
    connect(menuBar, SIGNAL(askForPasteCopiedNetworkElements()), centralWidget(), SLOT(pasteCopiedNetworkElements()));
    connect(centralWidget(), SIGNAL(pasteElementsStatusChanged(const bool&)), menuBar, SIGNAL(pasteElementsStatusChanged(const bool&)));
    connect(menuBar, SIGNAL(askForSelectAllElements()), centralWidget(), SLOT(selectAllElements()));
    connect(menuBar, SIGNAL(askForSelectAllElements(const QString&)), centralWidget(), SLOT(selectAllElements(const QString&)));
    connect(menuBar, SIGNAL(askForZoomIn()), centralWidget(), SLOT(zoomIn()));
    connect(menuBar, SIGNAL(askForZoomOut()), centralWidget(), SLOT(zoomOut()));
    connect(menuBar, SIGNAL(askForName()), centralWidget(), SLOT(getName()));
    connect(menuBar, SIGNAL(askForVersionNumber()), centralWidget(), SLOT(getVersionNumber()));
}

void MyMainWindow::closeEvent(QCloseEvent *event) {
    centralWidget()->close();
    QWidget::closeEvent(event);
}