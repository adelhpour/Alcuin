#include "negui_ellipse_style.h"
#include <QJsonObject>

// MyEllipseStyleBase

MyEllipseStyleBase::MyEllipseStyleBase(const QString& name) : My2DShapeStyleBase(name) {
    // border-width
    addParameter(new MyBorderWidthParameter());
    
    // border-color
    addParameter(new MyBorderColorParameter());
    
    // fill-color
    addParameter(new MyFillColorParameter());

    reset();
}

MyShapeStyleBase::SHAPE_STYLE MyEllipseStyleBase::type() {
    return ELLIPSE_SHAPE_STYLE;
}

const QRectF MyEllipseStyleBase::getShapeExtents() {
    QRectF extents;
    extents.setX(INT_MAX);
    extents.setY(INT_MAX);
    if (cx() - rx() < extents.x())
        extents.setX(cx() - rx());
    if (cy() - ry() < extents.y())
        extents.setY(cy() - ry());
    if (2 * rx() > extents.width())
        extents.setWidth(2 * rx());
    if (2 * ry() > extents.height())
        extents.setHeight(2 * ry());
    
    return extents;
}

void MyEllipseStyleBase::updateShapeExtents(const QRectF& extents) {
    setCx(extents.center().x());
    setCy(extents.center().y());
    setRx(0.5 * extents.width());
    setRy(0.5 * extents.height());
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
    My2DShapeStyleBase::read(json);
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
    My2DShapeStyleBase::write(json);
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
    addParameter(new MyNodeEllipsePositionalParameter("cx"));
    
    // cy
    addParameter(new MyNodeEllipsePositionalParameter("cy"));
    
    // rx
    addParameter(new MyNodeEllipseDimensionalParameter("rx"));
    
    // ry
    addParameter(new MyNodeEllipseDimensionalParameter("ry"));
    
    reset();
}

// MyArrowHeadEllipseStyle

MyArrowHeadEllipseStyle::MyArrowHeadEllipseStyle(const QString& name) : MyEllipseStyleBase(name) {
    // cx
    addParameter(new MyArrowHeadEllipseCxParameter());
    
    // cy
    addParameter(new MyArrowHeadEllipseCyParameter());
    
    // rx
    addParameter(new MyArrowHeadEllipseDimensionalParameter("rx"));
    
    // ry
    addParameter(new MyArrowHeadEllipseDimensionalParameter("ry"));
    
    reset();
}
