#include "negui_customized_interactor_widgets.h"
#include "negui_parameters.h"

#include <QGridLayout>

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

// MyTakeParameterDialog

MyTakeParameterDialog::MyTakeParameterDialog(const QString& name, const QJsonValue& defaultValue, QWidget *parent) : MyDialog(parent) {
    _parameter = createParameter(name, defaultValue);
    if (_parameter) {
        QGridLayout* contentLayout = (QGridLayout*)layout();
        contentLayout->addWidget(new MyLabel(name), contentLayout->rowCount(), 0);
        QWidget* inputWidget = ((MyParameterBase*)_parameter)->inputWidget();
        if (inputWidget)
            contentLayout->addWidget(inputWidget, contentLayout->rowCount() - 1, 1);
        setWindowTitle("Please enter the value for \"" + name + "\"");
        setButtons();
    }
}

const QJsonValue MyTakeParameterDialog::execute() {
    exec();
    ((MyParameterBase*)_parameter)->setDefaultValue();
    if (((MyParameterBase*)_parameter)->type() == MyParameterBase::DOUBLE_PARAMETER_TYPE)
        return ((MyDoubleParameter*)_parameter)->defaultValue();
    else if (((MyParameterBase*)_parameter)->type() == MyParameterBase::BOOLEAN_PARAMETER_TYPE)
        return ((MyBooleanParameter*)_parameter)->defaultValue();
    else if (((MyParameterBase*)_parameter)->type() == MyParameterBase::STRING_PARAMETER_TYPE)
        return ((MyStringParameter*)_parameter)->defaultValue();

    return "";
}

MyBase* MyTakeParameterDialog::createParameter(const QString& name, const QJsonValue& defaultValue) {
    MyParameterBase* parameter = NULL;
    if (defaultValue.isDouble()) {
        parameter = new MyDoubleParameter(name);
        ((MyDoubleParameter*)parameter)->setDefaultValue(defaultValue.toDouble());
    }
    else if (defaultValue.isString()) {
        parameter = new MyStringParameter(name);
        ((MyStringParameter*)parameter)->setDefaultValue(defaultValue.toString());
    }
    else if (defaultValue.isBool()) {
        parameter = new MyBooleanParameter(name);
        ((MyBooleanParameter*)parameter)->setDefaultValue(defaultValue.toBool());
    }

    return parameter;
}