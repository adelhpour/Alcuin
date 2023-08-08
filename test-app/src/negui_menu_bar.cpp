#include "negui_menu_bar.h"

#include <QAction>
#include <QMenu>

MyMenuBar::MyMenuBar(QWidget* parent) : QMenuBar(parent) {
    /// file menu
    QMenu* fileMenu = addMenu(tr("&File"));
    // new
    QAction* newAction = new QAction(tr("&New"), fileMenu);
    fileMenu->addAction(newAction);
    connect(newAction, &QAction::triggered, this, &MyMenuBar::askForSetNewNetworkCanvas);
    fileMenu->addSeparator();
    // open
    QAction* openAction = new QAction(tr("&Open"), fileMenu);
    fileMenu->addAction(openAction);
    //connect(openAction, &QAction::triggered, this, &MyMenuBar::askForSetNewNetworkCanvas);
    // open recent
    QAction* openRecentAction = new QAction(tr("&Open Recent"), fileMenu);
    fileMenu->addAction(openRecentAction);
    //connect(openRecentAction, &QAction::triggered, this, &MyMenuBar::askForSetNewNetworkCanvas);
    fileMenu->addSeparator();
    // save
    QAction* saveAction = new QAction(tr("&Save"), fileMenu);
    fileMenu->addAction(saveAction);
    //connect(saveAction, &QAction::triggered, this, &MyMenuBar::askForSetNewNetworkCanvas);
    // save as
    QAction* saveAsAction = new QAction(tr("&Save As"), fileMenu);
    fileMenu->addAction(saveAsAction);
    //connect(saveAsAction, &QAction::triggered, this, &MyMenuBar::askForSetNewNetworkCanvas);
    fileMenu->addSeparator();
    // export
    QAction* exportAction = new QAction(tr("&Export"), fileMenu);
    fileMenu->addAction(exportAction);
    //connect(exportAction, &QAction::triggered, this, &MyMenuBar::askForSetNewNetworkCanvas);
    fileMenu->addSeparator();
    // print
    QAction* printAction = new QAction(tr("&Print"), fileMenu);
    fileMenu->addAction(printAction);
    //connect(printAction, &QAction::triggered, this, &MyMenuBar::askForSetNewNetworkCanvas);
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
    editMenu->addAction(preferencesAction);
    //connect(preferencesAction, &QAction::triggered, this, &MyMenuBar::askForSetNewNetworkCanvas);

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
