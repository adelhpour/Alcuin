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
    void askForSetNewNetworkCanvas();
    void askForReadFromFile(const QString&);
    void askForSaveCurrentNetwork();
    void askForWriteDataToFile(const QString&);
    void askForWriteFigureToFile(const QString&);
    void askForTriggerUndoAction();
    void undoActionIsEnabled(const bool&);
    void askForTriggerRedoAction();
    void redoActionIsEnabled(const bool&);
    void askForSelectElements();
};

#endif
