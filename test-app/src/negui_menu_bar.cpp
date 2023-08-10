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
    const QStringList dataExportToolNames = askForListOfPluginItemNames("dataexporttool");
    for (const QString& dataExportToolName: dataExportToolNames) {
        QAction* saveAsAction = saveAsMenu->addAction(dataExportToolName);
        connect(saveAsAction, &QAction::triggered, this, [this, dataExportToolName] () { askForWriteDataToFile(dataExportToolName); });
    }
    fileMenu->addSeparator();
    // export
    QMenu* exportMenu = fileMenu->addMenu(tr("&Export"));
    const QStringList printExportToolNames = askForListOfPluginItemNames("printexporttool");
    for (const QString& printExportToolName: printExportToolNames) {
        QAction* exportAction = exportMenu->addAction(printExportToolName);
        connect(exportAction, &QAction::triggered, this, [this, printExportToolName] () { askForWriteFigureToFile(printExportToolName); });
    }
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
    undoAction->setEnabled(false);
    editMenu->addAction(undoAction);
    connect(undoAction, &QAction::triggered, this, &MyMenuBar::askForTriggerUndoAction);
    connect(this, &MyMenuBar::canUndoChanged, undoAction, [undoAction] (const bool& enabled) { undoAction->setEnabled(enabled); });
    // redo
    QAction* redoAction = new QAction(tr("&Redo"), editMenu);
    redoAction->setEnabled(false);
    editMenu->addAction(redoAction);
    connect(redoAction, &QAction::triggered, this, &MyMenuBar::askForTriggerRedoAction);
    connect(this, &MyMenuBar::canRedoChanged, redoAction, [redoAction] (const bool& enabled) { redoAction->setEnabled(enabled); });
    editMenu->addSeparator();
    // cut
    QAction* cutAction = new QAction(tr("&Cut"), editMenu);
    cutAction->setEnabled(false);
    editMenu->addAction(cutAction);
    connect(cutAction, &QAction::triggered, this, &MyMenuBar::askForCutSelectedNetworkElements);
    connect(this, &MyMenuBar::elementsCuttableStatusChanged, cutAction, [cutAction] (const bool& cuttable) { cutAction->setEnabled(cuttable); });
    // copy
    QAction* copyAction = new QAction(tr("&Copy"), editMenu);
    copyAction->setEnabled(false);
    editMenu->addAction(copyAction);
    connect(copyAction, &QAction::triggered, this, &MyMenuBar::askForCopySelectedNetworkElements);
    connect(this, &MyMenuBar::elementsCopyableStatusChanged, copyAction, [copyAction] (const bool& copyable) { copyAction->setEnabled(copyable); });
    // paste
    QAction* pasteAction = new QAction(tr("&Paste"), editMenu);
    pasteAction->setEnabled(false);
    editMenu->addAction(pasteAction);
    connect(pasteAction, &QAction::triggered, this, &MyMenuBar::askForPasteCopiedNetworkElements);
    connect(this, &MyMenuBar::pasteElementsStatusChanged, pasteAction, [pasteAction] (const bool& areCopied) { pasteAction->setEnabled(areCopied); });
    editMenu->addSeparator();
    // select all
    QAction* selectAllAction = new QAction(tr("&Select All"), editMenu);
    editMenu->addAction(selectAllAction);
    connect(selectAllAction, &QAction::triggered, this, QOverload<>::of(&MyMenuBar::askForSelectAllElements));
    // select all by category
    QMenu* selectMenu = editMenu->addMenu(tr("&Select All by Category"));
    QStringList elementStyleCategories = askForListOfPluginItemCategories("nodestyle");
    elementStyleCategories += askForListOfPluginItemCategories("edgestyle");
    elementStyleCategories.removeDuplicates();
    for (const QString& elementStyleCategory : elementStyleCategories) {
        QAction* selectAction = selectMenu->addAction(elementStyleCategory);
        connect(selectAction, &QAction::triggered, this, [this, elementStyleCategory] () { askForSelectAllElements(elementStyleCategory);  });
    }
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
