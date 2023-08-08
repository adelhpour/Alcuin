#include "negui_menu_bar.h"

#include <QAction>
#include <QMenu>

MyMenuBar::MyMenuBar(QWidget* parent) : QMenuBar(parent) {

}

void MyMenuBar::setMenus() {
    /// file menu
    QMenu* fileMenu = addMenu(tr("&File"));
    // new
    QAction* newAction = new QAction(tr("&New"), fileMenu);
    fileMenu->addAction(newAction);
    connect(newAction, &QAction::triggered, this, &MyMenuBar::askForSetNewNetworkCanvas);
    fileMenu->addSeparator();
    // open
    QMenu* openMenu = fileMenu->addMenu(tr("&Open"));
    const QStringList importToolNames = askForListOfPluginItemNames("importtool");
    for (const QString& importToolName: importToolNames) {
        QAction* openAction = openMenu->addAction(importToolName);
        connect(openAction, &QAction::triggered, this, [this, importToolName] () { askForReadFromFile(importToolName); });
    }
    // open recent
    QAction* openRecentAction = new QAction(tr("&Open Recent"), fileMenu);
    openRecentAction->setEnabled(false);
    fileMenu->addAction(openRecentAction);
    fileMenu->addSeparator();
    // save
    QAction* saveAction = new QAction(tr("&Save"), fileMenu);
    fileMenu->addAction(saveAction);
    connect(saveAction, &QAction::triggered, this, &MyMenuBar::askForSaveCurrentNetwork);
    // save as
    QMenu* saveAsMenu = fileMenu->addMenu(tr("&Save As"));
    const QStringList exportToolNames = askForListOfPluginItemNames("dataexporttool");
    for (const QString& exportToolName: exportToolNames) {
        QAction* saveAsAction = saveAsMenu->addAction(exportToolName);
        connect(saveAsAction, &QAction::triggered, this, [this, exportToolName] () { askForWriteDataToFile(exportToolName); });
    }
    fileMenu->addSeparator();
    // export
    QAction* exportAction = new QAction(tr("&Export"), fileMenu);
    fileMenu->addAction(exportAction);
    //connect(exportAction, &QAction::triggered, this, &MyMenuBar::askForSetNewNetworkCanvas);
    fileMenu->addSeparator();
    // print
    QAction* printAction = new QAction(tr("&Print"), fileMenu);
    printAction->setEnabled(false);
    fileMenu->addAction(printAction);
    fileMenu->addSeparator();
    // exit
    QAction* exitAction = new QAction(tr("&Exit"), fileMenu);
    fileMenu->addAction(exitAction);
    //connect(exitAction, &QAction::triggered, this, &MyMenuBar::askForSetNewNetworkCanvas);
    fileMenu->addSeparator();

    /// edit menu
    QMenu* editMenu = addMenu(tr("&Edit"));
    // undo
    QAction* undoAction = new QAction(tr("&Undo"), editMenu);
    editMenu->addAction(undoAction);
    //connect(undoAction, &QAction::triggered, this, &MyMenuBar::askForSetNewNetworkCanvas);
    // redo
    QAction* redoAction = new QAction(tr("&Redo"), editMenu);
    editMenu->addAction(redoAction);
    //connect(redoAction, &QAction::triggered, this, &MyMenuBar::askForSetNewNetworkCanvas);
    editMenu->addSeparator();
    // cut
    QAction* cutAction = new QAction(tr("&Cut"), editMenu);
    editMenu->addAction(cutAction);
    //connect(cutAction, &QAction::triggered, this, &MyMenuBar::askForSetNewNetworkCanvas);
    // copy
    QAction* copyAction = new QAction(tr("&Copy"), editMenu);
    editMenu->addAction(copyAction);
    //connect(copyAction, &QAction::triggered, this, &MyMenuBar::askForSetNewNetworkCanvas);
    // paste
    QAction* pasteAction = new QAction(tr("&Paste"), editMenu);
    editMenu->addAction(pasteAction);
    //connect(pasteAction, &QAction::triggered, this, &MyMenuBar::askForSetNewNetworkCanvas);
    editMenu->addSeparator();
    // select all
    QAction* selectAllAction = new QAction(tr("&Select All"), editMenu);
    editMenu->addAction(selectAllAction);
    connect(selectAllAction, &QAction::triggered, this, &MyMenuBar::askForSelectElements);
    // select all nodes
    QAction* selectAllNodesAction = new QAction(tr("&Select All Nodes"), editMenu);
    editMenu->addAction(selectAllNodesAction);
    //connect(selectAllNodesAction, &QAction::triggered, this, &MyMenuBar::askForSetNewNetworkCanvas);
    // select all reactions
    QAction* selectAllReactionsAction = new QAction(tr("&Select All Reactions"), editMenu);
    editMenu->addAction(selectAllReactionsAction);
    //connect(selectAllReactionsAction, &QAction::triggered, this, &MyMenuBar::askForSetNewNetworkCanvas);
    editMenu->addSeparator();
    // preferences
    QAction* preferencesAction = new QAction(tr("&Preferences"), editMenu);
    preferencesAction->setEnabled(false);
    editMenu->addAction(preferencesAction);

    /// help menu
    QMenu* helpMenu = addMenu(tr("&Help"));
    // list of shortcuts
    QAction* listOfShortcutsAction = new QAction(tr("&List of Shortcuts"), helpMenu);
    helpMenu->addAction(listOfShortcutsAction);
    connect(listOfShortcutsAction, &QAction::triggered, this, &MyMenuBar::askForSetNewNetworkCanvas);
    // about
    QAction* aboutAction = new QAction(tr("&About"), helpMenu);
    helpMenu->addAction(aboutAction);
    connect(aboutAction, &QAction::triggered, this, &MyMenuBar::askForSetNewNetworkCanvas);
}
