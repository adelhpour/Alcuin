#ifndef __NEGUI_CUSTOMIZED_INTERACTOR_WIDGETS_H
#define __NEGUI_CUSTOMIZED_INTERACTOR_WIDGETS_H

#include "negui_customized_common_widgets.h"
#include "negui_plugin_item_base.h"

#include <QUndoStack>
#include <QWidgetAction>
#include <QPushButton>
#include <QVBoxLayout>

class MyUndoStack : public QUndoStack {

public:

    MyUndoStack(QObject *parent = nullptr);

    void addCommand(QUndoCommand* command);

    void clear();
};

class MyChangeStageCommand : public QObject, public QUndoCommand {
    Q_OBJECT

public:

    MyChangeStageCommand(const QJsonObject& previousStageInfo, const QJsonObject& currentStageInfo, QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;

    const QJsonObject& previousStageInfo();
    const QJsonObject& currentStageInfo();

    signals:
            void askForCreateNetwork(const QJsonObject&);

protected:

    QJsonObject _previousStageInfo;
    QJsonObject _currentStageInfo;
};

class MyWidgetAction : public QWidgetAction {
    Q_OBJECT

public:

    MyWidgetAction(QObject* parent = nullptr);

    void setItems(QList<MyPluginItemBase*> items);

signals:
    void itemIsChosen(MyPluginItemBase*);

protected:
    QWidget* createItemPreviewWidget(QList<MyPluginItemBase*> items);
    QPushButton* createItemPreviewButton(MyPluginItemBase* item);
};

class MyItemPreviewButton : public QPushButton {

public:

    MyItemPreviewButton(MyPluginItemBase* item, QWidget *parent = nullptr);
};

class MyAutoSaveDialog : public MyDialog {
    Q_OBJECT

public:

    MyAutoSaveDialog(QWidget *parent = nullptr);
};

#endif
