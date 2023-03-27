#include "negui_shape_style_base.h"
#include <QJsonObject>

// MyShapeStyleBase

MyShapeStyleBase::MyShapeStyleBase(const QString& name) : MyBase(name) {

}

MyShapeStyleBase::~MyShapeStyleBase() {

}

const QList<MyParameterBase*>& MyShapeStyleBase::parameters() const {
    return _parameters;
}

MyParameterBase* MyShapeStyleBase::findParameter(const QString& name) const {
    for (MyParameterBase* parameter : qAsConst(parameters())) {
        if (parameter->name() == name) {
            return parameter;
        }
    }
    return NULL;
}

void MyShapeStyleBase::populateFeaturesMenu(QGridLayout* featureMenuLayout) {
    for (MyParameterBase* parameter : qAsConst(parameters())) {
        featureMenuLayout->addWidget(new MyLabel(parameter->name()), featureMenuLayout->rowCount(), 0);
        featureMenuLayout->addWidget(parameter->inputWidget(), featureMenuLayout->rowCount() - 1, 1);
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

