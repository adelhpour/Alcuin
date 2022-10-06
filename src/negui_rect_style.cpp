#include "negui_rect_style.h"

// MyRectStyleBase

MyRectStyleBase::MyRectStyleBase(const QString& name) : MyShapeStyleBase(name) {
    // stroke-width
    _parameters.push_back(new MyStrokeWidthParameter());
    
    // stroke
    _parameters.push_back(new MyStrokeParameter());
    
    // fill
    _parameters.push_back(new MyFillParameter());
    
    // rx
    _parameters.push_back(new MyCornerCurvatureParameter("rx"));
    
    // ry
    _parameters.push_back(new MyCornerCurvatureParameter("ry"));
}

MyShapeStyleBase::SHAPE_STYLE MyRectStyleBase::type() {
    return RECT_SHAPE_STYLE;
}

void MyRectStyleBase::setX(const qreal& x) const {
    MyParameterBase* parameter = findParameter("x");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(x);
}

const qreal MyRectStyleBase::x() const {
    MyParameterBase* parameter = findParameter("x");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyRectStyleBase::setY(const qreal& y) const {
    MyParameterBase* parameter = findParameter("y");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(y);
}

const qreal MyRectStyleBase::y() const {
    MyParameterBase* parameter = findParameter("y");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyRectStyleBase::setWidth(const qreal& width) const {
    MyParameterBase* parameter = findParameter("width");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(width);
}

const qreal MyRectStyleBase::width() const {
    MyParameterBase* parameter = findParameter("width");
    if (parameter)
        return ((MyDimensionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyRectStyleBase::setHeight(const qreal& height) const {
    MyParameterBase* parameter = findParameter("height");
    if (parameter)
        return ((MyPositionalParameter*)parameter)->setDefaultValue(height);
}

const qreal MyRectStyleBase::height() const {
    MyParameterBase* parameter = findParameter("height");
    if (parameter)
        return ((MyDimensionalParameter*)parameter)->defaultValue();
    
    return 0.000;
}

const qreal MyRectStyleBase::rx() const {
    MyParameterBase* parameter = findParameter("rx");
    if (parameter)
        return ((MyCornerCurvatureParameter*)parameter)->defaultValue();
    
    return 0.000;
}

const qreal MyRectStyleBase::ry() const {
    MyParameterBase* parameter = findParameter("ry");
    if (parameter)
        return ((MyCornerCurvatureParameter*)parameter)->defaultValue();
    
    return 0.000;
}

void MyRectStyleBase::read(const QJsonObject &json) {
    MyShapeStyleBase::read(json);
    MyParameterBase* parameter = NULL;
    
    // x
    if (json.contains("x") && json["x"].isDouble()) {
        parameter = findParameter("x");
        if (parameter)
            ((MyPositionalParameter*)parameter)->setDefaultValue(json["x"].toDouble());
    }
    
    // y
    if (json.contains("y") && json["y"].isDouble()) {
        parameter = findParameter("y");
        if (parameter)
            ((MyPositionalParameter*)parameter)->setDefaultValue(json["y"].toDouble());
    }
    
    // width
    if (json.contains("width") && json["width"].isDouble()) {
        parameter = findParameter("width");
        if (parameter)
            ((MyDimensionalParameter*)parameter)->setDefaultValue(json["width"].toDouble());
    }
    
    // height
    if (json.contains("height") && json["height"].isDouble()) {
        parameter = findParameter("height");
        if (parameter)
            ((MyDimensionalParameter*)parameter)->setDefaultValue(json["height"].toDouble());
    }
    
    // rx
    if (json.contains("rx") && json["rx"].isDouble()) {
        parameter = findParameter("rx");
        if (parameter)
            ((MyCornerCurvatureParameter*)parameter)->setDefaultValue(json["rx"].toDouble());
    }
    
    // ry
    if (json.contains("ry") && json["ry"].isDouble()) {
        parameter = findParameter("ry");
        if (parameter)
            ((MyCornerCurvatureParameter*)parameter)->setDefaultValue(json["ry"].toDouble());
    }
}

void MyRectStyleBase::write(QJsonObject &json) {
    MyShapeStyleBase::write(json);
    MyParameterBase* parameter = NULL;
    
    // x
    parameter = findParameter("x");
    if (parameter)
        json["x"] = ((MyPositionalParameter*)parameter)->defaultValue();
    
    // y
    parameter = findParameter("y");
    if (parameter)
        json["y"] = ((MyPositionalParameter*)parameter)->defaultValue();
    
    // width
    parameter = findParameter("width");
    if (parameter)
        json["width"] = ((MyDimensionalParameter*)parameter)->defaultValue();
    
    // height
    parameter = findParameter("height");
    if (parameter)
        json["height"] = ((MyDimensionalParameter*)parameter)->defaultValue();
    
    // rx
    parameter = findParameter("rx");
    if (parameter)
        json["rx"] = ((MyCornerCurvatureParameter*)parameter)->defaultValue();
    
    // ry
    parameter = findParameter("ry");
    if (parameter)
        json["ry"] = ((MyCornerCurvatureParameter*)parameter)->defaultValue();
}

// MyNodeRectStyle

MyNodeRectStyle::MyNodeRectStyle(const QString& name) : MyRectStyleBase(name) {
    // x
    _parameters.push_back(new MyPositionalParameter("x"));
    
    // y
    _parameters.push_back(new MyPositionalParameter("y"));
    
    // width
    _parameters.push_back(new MyNodeRectDimensionalParameter("width"));
    
    // height
    _parameters.push_back(new MyNodeRectDimensionalParameter("height"));
    
    reset();
}

// MyArrowHeadRectStyle

MyArrowHeadRectStyle::MyArrowHeadRectStyle(const QString& name) : MyRectStyleBase(name) {
    // x
    _parameters.push_back(new MyPositionalParameter("x"));
    
    // y
    _parameters.push_back(new MyPositionalParameter("y"));
    
    // width
    _parameters.push_back(new MyArrowHeadRectDimensionalParameter("width"));
    
    // height
    _parameters.push_back(new MyArrowHeadRectDimensionalParameter("height"));
    
    reset();
}
