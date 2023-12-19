#ifndef __NEGUI_MENUBAR_H
#define __NEGUI_MENUBAR_H

#include <QMenuBar>
#include <QJsonArray>

class MyMenuBar : public QMenuBar {
    Q_OBJECT

public:

    MyMenuBar(QWidget* parent = nullptr);

    void setMenus();

signals:

    const QJsonArray askForListOfPluginItemNames(const QString&);
    const QJsonArray askForListOfPluginItemCategories(const QString&);
    void askForSetNewNetworkCanvas();
    void askForCallPluginFunctions(const QString&);
    void askForSaveCurrentNetwork();
    void askForCloseTheApp();
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
    void askForSelectNetworkElements(const bool&);
    void askForSelectNetworkElementsOfCategory(const bool&, const QString&);
    void askForZoomIn();
    void askForZoomOut();
    QString askForName();
    QString askForVersionNumber();
};

const QStringList toStringList(const QJsonArray& json);

#endif
