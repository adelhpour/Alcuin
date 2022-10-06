#include "negui_shape_style_base.h"

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

const QPen MyShapeStyleBase::pen() const {
    QPen pen;
    
    MyParameterBase* parameter = NULL;
    // stroke-width
    parameter = findParameter("stroke-width");
    if (parameter)
        pen.setWidth(((MyStrokeWidthParameter*)parameter)->defaultValue());
    
    // stroke
    parameter = findParameter("stroke");
    if (parameter)
        pen.setColor(QColor(((MyStrokeParameter*)parameter)->defaultValue()));
    
    return pen;
}

const QPen MyShapeStyleBase::selectedPen() const {
    QPen pen;
    
    MyParameterBase* parameter = NULL;
    // stroke-width
    parameter = findParameter("stroke-width");
    if (parameter)
        pen.setWidth(((MyStrokeWidthParameter*)parameter)->defaultValue());
    
    // stroke
    pen.setColor(QColor("#4169e1"));
    
    return pen;
}

const QBrush MyShapeStyleBase::brush() const {
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    
    MyParameterBase* parameter = findParameter("fill");
    if (parameter)
        brush.setColor(QColor(((MyFillParameter*)parameter)->defaultValue()));
    
    return brush;
}

const QBrush MyShapeStyleBase::selectedBrush() const {
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor("#4169e1"));
    return brush;
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
    reset();
    MyParameterBase* parameter = NULL;
    
    // stroke-width
    if (json.contains("stroke-width") && json["stroke-width"].isDouble()) {
        parameter = findParameter("stroke-width");
        if (parameter)
            ((MyStrokeWidthParameter*)parameter)->setDefaultValue(json["stroke-width"].toInt());
    }
    
    // stroke
    if (json.contains("stroke") && json["stroke"].isString()) {
        parameter = findParameter("stroke");
        if (parameter)
            ((MyStrokeParameter*)parameter)->setDefaultValue(json["stroke"].toString());
    }
    
    // fill
    if (json.contains("fill") && json["fill"].isString()) {
        parameter = findParameter("fill");
        if (parameter)
            ((MyFillParameter*)parameter)->setDefaultValue(json["fill"].toString());
    }
}

void MyShapeStyleBase::write(QJsonObject &json) {
    json["shape"] = name();
    MyParameterBase* parameter = NULL;
    
    // stroke-width
    parameter = findParameter("stroke-width");
    if (parameter)
        json["stroke-width"] = ((MyStrokeWidthParameter*)parameter)->defaultValue();
    
    // stroke
    parameter = findParameter("stroke");
    if (parameter)
        json["stroke"] = ((MyStrokeParameter*)parameter)->defaultValue();
    
    // fill
    parameter = findParameter("fill");
    if (parameter)
        json["fill"] = ((MyFillParameter*)parameter)->defaultValue();
}
