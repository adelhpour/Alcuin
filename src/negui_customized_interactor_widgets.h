#ifndef __NEGUI_CUSTOMIZED_INTERACTOR_WIDGETS_H
#define __NEGUI_CUSTOMIZED_INTERACTOR_WIDGETS_H

#include <QUndoStack>
#include <QJsonObject>
#include <QDialog>
#include <QMessageBox>

#include "negui_base.h"
#include "negui_customized_common_widgets.h"

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

class MyTakeParameterDialog : public MyDialog {
    Q_OBJECT

public:

    MyTakeParameterDialog(const QString& name, const QJsonValue& defaultValue, QWidget *parent = nullptr);

    const QJsonValue execute();

protected:

    MyBase* createParameter(const QString& name, const QJsonValue& defaultValue);

    MyBase* _parameter;
};

class MyShowParameterValueMessageBox : public QMessageBox {
    Q_OBJECT

public:

    MyShowParameterValueMessageBox(const QString& name, const QJsonValue& value, QWidget *parent = nullptr);
};

#endif
