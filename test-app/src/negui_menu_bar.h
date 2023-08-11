#ifndef __NEGUI_MENUBAR_H
#define __NEGUI_MENUBAR_H

#include <QMenuBar>

class MyMenuBar : public QMenuBar {
    Q_OBJECT

public:

    MyMenuBar(QWidget* parent = nullptr);

    void setMenus();

signals:

    const QStringList askForListOfPluginItemNames(const QString&);
    const QStringList askForListOfPluginItemCategories(const QString&);
    void askForSetNewNetworkCanvas();
    void askForReadFromFile(const QString&);
    void askForSaveCurrentNetwork();
    void askForWriteDataToFile(const QString&);
    void askForWriteFigureToFile(const QString&);
    void askForCloseTheWidget();
    void askForTriggerUndoAction();
    void canUndoChanged(const bool&);
    void askForTriggerRedoAction();
    void canRedoChanged(const bool&);
    void askForCutSelectedNetworkElements();
    void elementsCuttableStatusChanged(const bool&);
    void askForCopySelectedNetworkElements();
    void elementsCopyableStatusChanged(const bool&);
    void askForPasteCopiedNetworkElements();
    void pasteElementsStatusChanged(const bool&);
    void askForSelectAllElements();
    void askForSelectAllElements(const QString&);
    QString askForName();
    QString askForVersionNumber();
};

#endif
