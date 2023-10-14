#include "negui_customized_interactor_widgets.h"

// MyUndoStack

MyUndoStack::MyUndoStack(QObject *parent) : QUndoStack(parent) {

}

void MyUndoStack::addCommand(QUndoCommand* command) {
    push(command);
}

void MyUndoStack::clear() {
    QUndoStack::clear();
}

// MyChangeStageCommand

MyChangeStageCommand::MyChangeStageCommand(const QJsonObject& previousStageInfo, const QJsonObject& currentStageInfo, QUndoCommand* parent) : QUndoCommand(parent) {
    _previousStageInfo = previousStageInfo;
    _currentStageInfo = currentStageInfo;
}

void MyChangeStageCommand::undo() {
    emit askForCreateNetwork(_previousStageInfo);
}

void MyChangeStageCommand::redo() {
    emit askForCreateNetwork(_currentStageInfo);
}

const QJsonObject& MyChangeStageCommand::previousStageInfo() {
    return _previousStageInfo;
}

const QJsonObject& MyChangeStageCommand::currentStageInfo() {
    return _currentStageInfo;
}