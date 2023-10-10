#ifndef __NEGUI_CUSTOMIZED_INTERACTOR_WIDGETS_H
#define __NEGUI_CUSTOMIZED_INTERACTOR_WIDGETS_H

#include <QUndoStack>
#include <QJsonObject>

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

#endif
