#include "negui_autolayout_engines.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QGridLayout>

// MyAutoLayoutEngine

MyAutoLayoutEngine::MyAutoLayoutEngine(const QString& name) : MyPluginItemBase(name) {
    _iconSize = QSize(75, 25);
}

MyAutoLayoutEngine::~MyAutoLayoutEngine() {
    clearParameters();
}

const QString MyAutoLayoutEngine::type() const {
    return "autolayoutengine";
}

void MyAutoLayoutEngine::addParameter(MyParameterBase* p) {
    if (p)
        _parameters.push_back(p);
}

void MyAutoLayoutEngine::removeParameter(MyParameterBase* p) {
    if (p)
        _parameters.removeOne(p);
}

QList<MyParameterBase*>& MyAutoLayoutEngine::parameters() {
    return _parameters;
}

int MyAutoLayoutEngine::takeParameters() {
    if (parameters().size()) {
        MyDialog* parameterSetDialog = new MyParameterSetDialog(parameters());
        parameterSetDialog->setWindowTitle(name() + " AutoLayout Engine");
        parameterSetDialog->setButtons();
        if (parameterSetDialog->exec() == QDialog::Rejected)
            return -1;
    }

    return 0;
}

void MyAutoLayoutEngine::clearParameters() {
    while(_parameters.size())
        delete _parameters.at(0);
    _parameters.clear();
}

const QIcon MyAutoLayoutEngine::icon() {
    QPixmap pixMap(iconDirectory());
    return QIcon(pixMap);
}

const QString& MyAutoLayoutEngine::iconDirectory() const {
    return _iconDirectory;
}

void MyAutoLayoutEngine::read(const QJsonObject &json) {
    // parameters
    clearParameters();
    if (json.contains("parameters") && json["parameters"].isArray()) {
        QJsonArray parametersArray = json["parameters"].toArray();
        for (int parameterIndex = 0; parameterIndex < parametersArray.size(); ++parameterIndex) {
            QJsonObject parameterObject = parametersArray[parameterIndex].toObject();
            if (parameterObject.contains("parameter") && parameterObject["parameter"].isString() && parameterObject.contains("type") && parameterObject["type"].isString()) {
                MyParameterBase* parameter = NULL;
                if (parameterObject["type"].toString() == "double")
                    parameter = new MyDoubleParameter(parameterObject["parameter"].toString());
                else if (parameterObject["type"].toString() == "integer")
                    parameter = new MyIntegerParameter(parameterObject["parameter"].toString());
                else if (parameterObject["type"].toString() == "boolean")
                    parameter = new MyBooleanParameter(parameterObject["parameter"].toString());
                else if (parameterObject["type"].toString() == "string")
                    parameter = new MyStringParameter(parameterObject["parameter"].toString());
                else if (parameterObject["type"].toString() == "nominal")
                    parameter = new MyNominalParameter(parameterObject["parameter"].toString());

                if (parameter) {
                    parameter->read(parameterObject);
                    addParameter(parameter);
                }
            }
        }
    }

    // icon directory
    if (json.contains("icon-file") && json["icon-file"].isString())
        _iconDirectory = json["icon-file"].toString();
}

void MyAutoLayoutEngine::write(QJsonObject &json) {
    // name
    json["name"] = name();

    // parameters
    QJsonArray parametersArray;
    for (MyParameterBase *parameter : qAsConst(parameters())) {
        QJsonObject parameterObject;
        parameter->write(parameterObject);
        parametersArray.append(parameterObject);
    }
    json["parameters"] = parametersArray;

    // icon directory
    json["icon-file"] = iconDirectory();
}

// MyParameterSetDialog

MyParameterSetDialog::MyParameterSetDialog(QList<MyParameterBase*> parameters, QWidget *parent) : MyDialog(parent) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    QWidget* inputWidget = NULL;
    for (MyParameterBase *parameter : qAsConst(parameters)) {
        contentLayout->addWidget(new MyLabel(parameter->name()), contentLayout->rowCount(), 0);
        inputWidget = parameter->inputWidget();
        if (inputWidget)
            contentLayout->addWidget(inputWidget, contentLayout->rowCount() - 1, 1);
    }
}