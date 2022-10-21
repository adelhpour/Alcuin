#include "negui_ellipse_style.h"
#include <QJsonObject>

// MyEllipseStyleBase

MyEllipseStyleBase::MyEllipseStyleBase(const QString& name) : MyShapeStyleBase(name) {
    // stroke-width
    _parameters.push_back(new MyStrokeWidthParameter());
    
    // stroke
    _parameters.push_back(new MyStrokeParameter());
    
    // fill
    _parameters.push_back(new MyFillParameter());
}

MyShapeStyleBase::SHAPE_STYLE MyEllipseStyleBase::type() {
    return ELLIPSE_SHAPE_STYLE;
}

void MyEllipseStyleBase::setCx(const qreal& cx) const {
    MyParameterBase* parameter = findParameter("cx");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(cx);
}

const qreal MyEllipseStyleBase::cx() const {
    MyParameterBase* parameter = findParameter("cx");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyEllipseStyleBase::setCy(const qreal& cy) const {
    MyParameterBase* parameter = findParameter("cy");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(cy);
}

const qreal MyEllipseStyleBase::cy() const {
    MyParameterBase* parameter = findParameter("cy");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyEllipseStyleBase::setRx(const qreal& rx) const {
    MyParameterBase* parameter = findParameter("rx");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(rx);
}

const qreal MyEllipseStyleBase::rx() const {
    MyParameterBase* parameter = findParameter("rx");
    if (parameter)
        return ((MyDimensionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyEllipseStyleBase::setRy(const qreal& ry) const {
    MyParameterBase* parameter = findParameter("ry");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(ry);
}

const qreal MyEllipseStyleBase::ry() const {
    MyParameterBase* parameter = findParameter("ry");
    if (parameter)
        return ((MyDimensionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyEllipseStyleBase::read(const QJsonObject &json) {
    MyShapeStyleBase::read(json);
    MyParameterBase* parameter = NULL;
    
    // cx
    if (json.contains("cx") && json["cx"].isDouble()) {
        parameter = findParameter("cx");
        if (parameter)
            ((MyPositionalParameter*)parameter)->setDefaultValue(json["cx"].toDouble());
    }
    
    // cy
    if (json.contains("cy") && json["cy"].isDouble()) {
        parameter = findParameter("cy");
        if (parameter)
            ((MyPositionalParameter*)parameter)->setDefaultValue(json["cy"].toDouble());
    }
    
    // rx
    if (json.contains("rx") && json["rx"].isDouble()) {
        parameter = findParameter("rx");
        if (parameter)
            ((MyDimensionalParameter*)parameter)->setDefaultValue(json["rx"].toDouble());
    }
    
    // ry
    if (json.contains("ry") && json["ry"].isDouble()) {
        parameter = findParameter("ry");
        if (parameter)
            ((MyDimensionalParameter*)parameter)->setDefaultValue(json["ry"].toDouble());
    }
}

void MyEllipseStyleBase::write(QJsonObject &json) {
    MyShapeStyleBase::write(json);
    MyParameterBase* parameter = NULL;
    
    // cx
    parameter = findParameter("cx");
    if (parameter)
        json["cx"] = ((MyPositionalParameter*)parameter)->defaultValue();
    
    // cy
    parameter = findParameter("cy");
    if (parameter)
        json["cy"] = ((MyPositionalParameter*)parameter)->defaultValue();
    
    // rx
    parameter = findParameter("rx");
    if (parameter)
        json["rx"] = ((MyDimensionalParameter*)parameter)->defaultValue();
    
    // cy
    parameter = findParameter("ry");
    if (parameter)
        json["ry"] = ((MyDimensionalParameter*)parameter)->defaultValue();
}

// MyNodeEllipseStyle

MyNodeEllipseStyle::MyNodeEllipseStyle(const QString& name) : MyEllipseStyleBase(name) {
    // cx
    _parameters.push_back(new MyNodeEllipsePositionalParameter("cx"));
    
    // cy
    _parameters.push_back(new MyNodeEllipsePositionalParameter("cy"));
    
    // rx
    _parameters.push_back(new MyDimensionalParameter("rx"));
    
    // ry
    _parameters.push_back(new MyDimensionalParameter("ry"));
    
    reset();
}

// MyArrowHeadEllipseStyle

MyArrowHeadEllipseStyle::MyArrowHeadEllipseStyle(const QString& name) : MyEllipseStyleBase(name) {
    // cx
    _parameters.push_back(new MyArrowHeadEllipsePositionalParameter("cx"));
    
    // cy
    _parameters.push_back(new MyArrowHeadEllipsePositionalParameter("cy"));
    
    // rx
    _parameters.push_back(new MyArrowHeadEllipseDimensionalParameter("rx"));
    
    // ry
    _parameters.push_back(new MyArrowHeadEllipseDimensionalParameter("ry"));
    
    reset();
}
