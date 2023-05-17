#ifndef __NEGUI_CHANGE_STAGE_COMMAND_H
#define __NEGUI_CHANGE_STAGE_COMMAND_H

#include <QUndoCommand>
#include <QJsonObject>

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
