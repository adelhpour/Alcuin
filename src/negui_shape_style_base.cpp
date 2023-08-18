#include "negui_shape_style_base.h"
#include "negui_customized_common_widgets.h"

#include <QJsonObject>
#include <QGridLayout>

// MyShapeStyleBase

MyShapeStyleBase::MyShapeStyleBase(const QString& name) : MyBase(name) {

}

MyShapeStyleBase::~MyShapeStyleBase() {

}

const QList<MyParameterBase*>& MyShapeStyleBase::parameters() const {
    return _parameters;
}

void MyShapeStyleBase::addParameter(MyParameterBase* parameter) {
    _parameters.push_back(parameter);
    connect(parameter, SIGNAL(isUpdated()), this, SIGNAL(isUpdated()));
}

void MyShapeStyleBase::addParameterToTheBeginningOfTheList(MyParameterBase* parameter) {
    _parameters.push_front(parameter);
    connect(parameter, SIGNAL(isUpdated()), this, SIGNAL(isUpdated()));
}

MyParameterBase* MyShapeStyleBase::findParameter(const QString& name) const {
    for (MyParameterBase* parameter : qAsConst(parameters())) {
        if (parameter->name() == name) {
            return parameter;
        }
    }
    return NULL;
}

void MyShapeStyleBase::populateFeaturesMenu(QLayout* featureMenuLayout) {
    for (MyParameterBase* parameter : qAsConst(parameters())) {
        ((QGridLayout*)featureMenuLayout)->addWidget(new MyLabel(parameter->name()), ((QGridLayout*)featureMenuLayout)->rowCount(), 0, Qt::AlignLeft);
        ((QGridLayout*)featureMenuLayout)->addWidget(parameter->inputWidget(), ((QGridLayout*)featureMenuLayout)->rowCount() - 1, 1, Qt::AlignRight);
    }
}

void MyShapeStyleBase::updateFeatures() {
    for (MyParameterBase* parameter : qAsConst(parameters()))
        parameter->setDefaultValue();
}

void MyShapeStyleBase::reset() {
    for (MyParameterBase* parameter : qAsConst(parameters()))
        parameter->reset();
}

void MyShapeStyleBase::read(const QJsonObject &json) {
    if (json.contains("shape") && json["shape"].isString())
        _name = json["shape"].toString();
    reset();
}

void MyShapeStyleBase::write(QJsonObject &json) {
    json["shape"] = name();
}

