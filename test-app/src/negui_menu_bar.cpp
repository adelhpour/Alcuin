#include "negui_menu_bar.h"
#include "negui_about_message_box.h"

#include <QAction>
#include <QMenu>

MyMenuBar::MyMenuBar(QWidget* parent) : QMenuBar(parent) {

}

void MyMenuBar::setMenus() {
    /// file menu
    QMenu* fileMenu = addMenu(tr("&File"));
    // new
    QAction* newAction = new QAction(tr("&New"), fileMenu);
    newAction->setShortcut(QKeySequence::New);
    fileMenu->addAction(newAction);
    connect(newAction, &QAction::triggered, this, &MyMenuBar::askForSetNewNetworkCanvas);
    fileMenu->addSeparator();
    // open
    QMenu* openMenu = fileMenu->addMenu(tr("&Open"));
    QStringList pluginItemNames = toStringList(askForListOfPluginItemNames("open"));
    for (const QString& pluginItemName: pluginItemNames) {
        QAction* openAction = openMenu->addAction(pluginItemName);
        connect(openAction, &QAction::triggered, this, [this, pluginItemName] () { askForCallPluginFunctions(pluginItemName); });
    }
    // open recent
    QAction* openRecentAction = new QAction(tr("&Open Recent"), fileMenu);
    openRecentAction->setEnabled(false);
    fileMenu->addAction(openRecentAction);
    fileMenu->addSeparator();
    // save
    QAction* saveAction = new QAction(tr("&Save"), fileMenu);
    saveAction->setShortcut(QKeySequence::Save);
    fileMenu->addAction(saveAction);
    pluginItemNames = toStringList(askForListOfPluginItemNames("save"));
    if (pluginItemNames.size())
        connect(saveAction, &QAction::triggered, this, [this, pluginItemNames] () { askForCallPluginFunctions(pluginItemNames.at(0)); });
    // save as
    QMenu* saveAsMenu = fileMenu->addMenu(tr("&Save As"));
    pluginItemNames = toStringList(askForListOfPluginItemNames("saveAs"));
    for (const QString& pluginItemName: pluginItemNames) {
        QAction* saveAsAction = saveAsMenu->addAction(pluginItemName);
        connect(saveAsAction, &QAction::triggered, this, [this, pluginItemName] () { askForCallPluginFunctions(pluginItemName); });
    }
    fileMenu->addSeparator();
    // export
    QMenu* exportMenu = fileMenu->addMenu(tr("&Export"));
    pluginItemNames = toStringList(askForListOfPluginItemNames("export"));
    for (const QString& pluginItemName: pluginItemNames) {
        QAction* exportAction = exportMenu->addAction(pluginItemName);
        connect(exportAction, &QAction::triggered, this, [this, pluginItemName] () { askForCallPluginFunctions(pluginItemName); });
    }
    fileMenu->addSeparator();
    // print
    QAction* printAction = new QAction(tr("&Print"), fileMenu);
    printAction->setShortcut(QKeySequence::Print);
    printAction->setEnabled(false);
    fileMenu->addAction(printAction);
    fileMenu->addSeparator();
    // exit
    QAction* exitAction = new QAction(tr("&Exit"), fileMenu);
    exitAction->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(exitAction);
    connect(exitAction, &QAction::triggered, this, &MyMenuBar::askForCloseTheApp);
    fileMenu->addSeparator();

    /// edit menu
    QMenu* editMenu = addMenu(tr("&Edit"));
    // undo
    QAction* undoAction = new QAction(tr("&Undo"), editMenu);
    undoAction->setShortcut(QKeySequence::Undo);
    undoAction->setEnabled(false);
    editMenu->addAction(undoAction);
    connect(undoAction, &QAction::triggered, this, &MyMenuBar::askForTriggerUndoAction);
    connect(this, &MyMenuBar::canUndoChanged, undoAction, [undoAction] (const bool& enabled) { undoAction->setEnabled(enabled); });
    // redo
    QAction* redoAction = new QAction(tr("&Redo"), editMenu);
    redoAction->setShortcut(QKeySequence::Redo);
    redoAction->setEnabled(false);
    editMenu->addAction(redoAction);
    connect(redoAction, &QAction::triggered, this, &MyMenuBar::askForTriggerRedoAction);
    connect(this, &MyMenuBar::canRedoChanged, redoAction, [redoAction] (const bool& enabled) { redoAction->setEnabled(enabled); });
    editMenu->addSeparator();
    // cut
    QAction* cutAction = new QAction(tr("&Cut"), editMenu);
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setEnabled(false);
    editMenu->addAction(cutAction);
    connect(cutAction, &QAction::triggered, this, &MyMenuBar::askForCutSelectedNetworkElements);
    connect(this, &MyMenuBar::elementsCuttableStatusChanged, cutAction, [cutAction] (const bool& cuttable) { cutAction->setEnabled(cuttable); });
    // copy
    QAction* copyAction = new QAction(tr("&Copy"), editMenu);
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setEnabled(false);
    editMenu->addAction(copyAction);
    connect(copyAction, &QAction::triggered, this, &MyMenuBar::askForCopySelectedNetworkElements);
    connect(this, &MyMenuBar::elementsCopyableStatusChanged, copyAction, [copyAction] (const bool& copyable) { copyAction->setEnabled(copyable); });
    // paste
    QAction* pasteAction = new QAction(tr("&Paste"), editMenu);
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setEnabled(false);
    editMenu->addAction(pasteAction);
    connect(pasteAction, &QAction::triggered, this, &MyMenuBar::askForPasteCopiedNetworkElements);
    connect(this, &MyMenuBar::pasteElementsStatusChanged, pasteAction, [pasteAction] (const bool& areCopied) { pasteAction->setEnabled(areCopied); });
    editMenu->addSeparator();
    // select all
    QAction* selectAllAction = new QAction(tr("&Select All"), editMenu);
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    editMenu->addAction(selectAllAction);
    connect(selectAllAction, &QAction::triggered, this, [this] () { askForSelectNetworkElements(true); });
    // select all by category
    QMenu* selectMenu = editMenu->addMenu(tr("&Select All by Category"));
    QStringList elementStyleCategories = toStringList(askForListOfPluginItemCategories("nodestyle"));
    elementStyleCategories += toStringList(askForListOfPluginItemCategories("edgestyle"));
    elementStyleCategories.removeDuplicates();
    for (const QString& elementStyleCategory : elementStyleCategories) {
        QAction* selectAction = selectMenu->addAction(elementStyleCategory);
        connect(selectAction, &QAction::triggered, this, [this, elementStyleCategory] () { askForSelectNetworkElementsOfCategory(true, elementStyleCategory);  });
    }
    // zoom in
    QAction* zoomInAction = new QAction(tr("&Zoom In"), editMenu);
    zoomInAction->setShortcut(QKeySequence::ZoomIn);
    editMenu->addAction(zoomInAction);
    connect(zoomInAction, &QAction::triggered, this, &MyMenuBar::askForZoomIn);
    // zoom out
    QAction* zoomOutAction = new QAction(tr("&Zoom Out"), editMenu);
    zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    editMenu->addAction(zoomOutAction);
    connect(zoomOutAction, &QAction::triggered, this, &MyMenuBar::askForZoomOut);
    editMenu->addSeparator();
    // preferences
    QAction* preferencesAction = new QAction(tr("&Preferences"), editMenu);
    preferencesAction->setShortcut(QKeySequence::Preferences);
    preferencesAction->setEnabled(false);
    editMenu->addAction(preferencesAction);

    /// help menu
    QMenu* helpMenu = addMenu(tr("&Help"));
    // about
    QAction* aboutAction = new QAction(tr("&About"), helpMenu);
    aboutAction->setShortcut(QKeySequence::WhatsThis);
    helpMenu->addAction(aboutAction);
    connect(aboutAction, &QAction::triggered, this, [this] () {
        QMessageBox* infoMessageBox = new MyAboutMessageBox(askForName(), askForVersionNumber());
        infoMessageBox->exec();} );
}

const QStringList toStringList(const QJsonArray& json) {
    QStringList stringList;
    for (const QJsonValue& element : json) {
        if (element.isString())
            stringList << element.toString();
    }

    return stringList;
}