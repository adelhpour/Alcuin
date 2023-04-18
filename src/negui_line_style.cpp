#include "negui_line_style.h"
#include <QJsonObject>

// MyLineStyle

MyLineStyle::MyLineStyle(const QString& name) : My1DShapeStyleBase(name) {
    // stroke-width
    addParameter(new MyStrokeWidthParameter());
    
    // stroke
    addParameter(new MyStrokeParameter());
    
    // control point 1
    addParameter(new MyBasePointParameter("ControlPoint1"));
    
    // control point 2
    addParameter(new MyBasePointParameter("ControlPoint2"));

    reset();
}

MyShapeStyleBase::SHAPE_STYLE MyLineStyle::type() {
    return LINE_SHAPE_STYLE;
}

const QRectF MyLineStyle::getShapeExtents() {
    return QRectF(0.0, 0.0, 0.0, 0.0);
}

void MyLineStyle::setRelativeP1(const QPointF& relativeP1) const {
    QPoint point;
    MyParameterBase* parameter = findParameter("ControlPoint1");
    if (parameter) {
        ((MyRelativePointParameter*)parameter)->setDefaultValueX(relativeP1.x());
        ((MyRelativePointParameter*)parameter)->setDefaultValueY(relativeP1.y());
    }
}

const QPoint MyLineStyle::relativeP1() const {
    QPoint point;
    MyParameterBase* parameter = findParameter("ControlPoint1");
    if (parameter)
        point = QPoint(((MyRelativePointParameter*)parameter)->defaultValueX(), ((MyRelativePointParameter*)parameter)->defaultValueY());
    
    return point;
}

void MyLineStyle::setRelativeP2(const QPointF& relativeP2) const {
    QPoint point;
    MyParameterBase* parameter = findParameter("ControlPoint2");
    if (parameter) {
        ((MyRelativePointParameter*)parameter)->setDefaultValueX(relativeP2.x());
        ((MyRelativePointParameter*)parameter)->setDefaultValueY(relativeP2.y());
    }
}

const QPoint MyLineStyle::relativeP2() const {
    QPoint point;
    MyParameterBase* parameter = findParameter("ControlPoint2");
    if (parameter)
        point = QPoint(((MyRelativePointParameter*)parameter)->defaultValueX(), ((MyRelativePointParameter*)parameter)->defaultValueY());
    
    return point;
}

void MyLineStyle::read(const QJsonObject &json) {
    My1DShapeStyleBase::read(json);
    MyParameterBase* parameter = NULL;
    
    // control p1
    if (json.contains("p1") && json["p1"].isObject()) {
        QJsonObject p1Object = json["p1"].toObject();
        parameter = findParameter("ControlPoint1");
        
        // x
        if (p1Object.contains("x") && p1Object["x"].isDouble())
            ((MyRelativePointParameter*)parameter)->setDefaultValueX(p1Object["x"].toInt());
        
        // y
        if (p1Object.contains("y") && p1Object["y"].isDouble())
            ((MyRelativePointParameter*)parameter)->setDefaultValueY(p1Object["y"].toInt());
    }
    
    // control p2
    if (json.contains("p2") && json["p2"].isObject()) {
        QJsonObject p2Object = json["p2"].toObject();
        parameter = findParameter("ControlPoint2");
        
        // x
        if (p2Object.contains("x") && p2Object["x"].isDouble())
            ((MyRelativePointParameter*)parameter)->setDefaultValueX(p2Object["x"].toInt());
        
        // y
        if (p2Object.contains("y") && p2Object["y"].isDouble())
            ((MyRelativePointParameter*)parameter)->setDefaultValueY(p2Object["y"].toInt());
    }
}

void MyLineStyle::write(QJsonObject &json) {
    My1DShapeStyleBase::write(json);
    MyParameterBase* parameter = NULL;
    
    // relative p1
    parameter = findParameter("ControlPoint1");
    if (parameter) {
        QJsonObject p1Object;
        p1Object["x"] = ((MyRelativePointParameter*)parameter)->defaultValueX();
        p1Object["y"] = ((MyRelativePointParameter*)parameter)->defaultValueY();
        json["p1"] = p1Object;
    }
    
    // control p2
    parameter = findParameter("ControlPoint2");
    if (parameter) {
        QJsonObject p2Object;
        p2Object["x"] = ((MyRelativePointParameter*)parameter)->defaultValueX();
        p2Object["y"] = ((MyRelativePointParameter*)parameter)->defaultValueY();
        json["p2"] = p2Object;
    }
}
