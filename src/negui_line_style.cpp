#include "negui_line_style.h"
#include <QJsonObject>

// MyLineStyleBase

MyLineStyleBase::MyLineStyleBase(const QString& name) : My1DShapeStyleBase(name) {
    // border-width
    addParameter(new MyBorderWidthParameter());
    
    // border-color
    addParameter(new MyBorderColorParameter());
    
    // control point 1
    addParameter(new MyBasePointParameter("ControlPoint1"));
    
    // control point 2
    addParameter(new MyBasePointParameter("ControlPoint2"));

    reset();
}

MyShapeStyleBase::SHAPE_STYLE MyLineStyleBase::type() {
    return LINE_SHAPE_STYLE;
}

const QRectF MyLineStyleBase::getShapeExtents() {
    return QRectF(0.0, 0.0, 0.0, 0.0);
}

void MyLineStyleBase::setRelativeP1(const QPointF& relativeP1) const {
    QPoint point;
    MyParameterBase* parameter = findParameter("ControlPoint1");
    if (parameter) {
        ((MyRelativePointParameter*)parameter)->setDefaultValueX(relativeP1.x());
        ((MyRelativePointParameter*)parameter)->setDefaultValueY(relativeP1.y());
    }
}

const QPoint MyLineStyleBase::relativeP1() const {
    QPoint point;
    MyParameterBase* parameter = findParameter("ControlPoint1");
    if (parameter)
        point = QPoint(((MyRelativePointParameter*)parameter)->defaultValueX(), ((MyRelativePointParameter*)parameter)->defaultValueY());
    
    return point;
}

void MyLineStyleBase::setRelativeP2(const QPointF& relativeP2) const {
    QPoint point;
    MyParameterBase* parameter = findParameter("ControlPoint2");
    if (parameter) {
        ((MyRelativePointParameter*)parameter)->setDefaultValueX(relativeP2.x());
        ((MyRelativePointParameter*)parameter)->setDefaultValueY(relativeP2.y());
    }
}

const QPoint MyLineStyleBase::relativeP2() const {
    QPoint point;
    MyParameterBase* parameter = findParameter("ControlPoint2");
    if (parameter)
        point = QPoint(((MyRelativePointParameter*)parameter)->defaultValueX(), ((MyRelativePointParameter*)parameter)->defaultValueY());
    
    return point;
}

void MyLineStyleBase::read(const QJsonObject &json) {
    My1DShapeStyleBase::read(json);
    MyParameterBase* parameter = NULL;
    
    // control p1
    if (json.contains("p1") && json["p1"].isObject()) {
        QJsonObject p1Object = json["p1"].toObject();
        parameter = findParameter("ControlPoint1");
        
        // x
        if (p1Object.contains("x") && p1Object["x"].isDouble())
            ((MyRelativePointParameter*)parameter)->setDefaultValueX(p1Object["x"].toDouble());
        
        // y
        if (p1Object.contains("y") && p1Object["y"].isDouble())
            ((MyRelativePointParameter*)parameter)->setDefaultValueY(p1Object["y"].toDouble());
    }
    
    // control p2
    if (json.contains("p2") && json["p2"].isObject()) {
        QJsonObject p2Object = json["p2"].toObject();
        parameter = findParameter("ControlPoint2");
        
        // x
        if (p2Object.contains("x") && p2Object["x"].isDouble())
            ((MyRelativePointParameter*)parameter)->setDefaultValueX(p2Object["x"].toDouble());
        
        // y
        if (p2Object.contains("y") && p2Object["y"].isDouble())
            ((MyRelativePointParameter*)parameter)->setDefaultValueY(p2Object["y"].toDouble());
    }
}

void MyLineStyleBase::write(QJsonObject &json) {
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

// MyClassicLineStyle

MyClassicLineStyle::MyClassicLineStyle(const QString& name) : MyLineStyleBase(name) {

}

MyLineStyleBase::LINE_STYLE_TYPE MyClassicLineStyle::lineType() {
    return CLASSIC_LINE_STYLE;
}

// MyConnectedToCentroidShapeLineStyleBase

MyConnectedToCentroidShapeLineStyleBase::MyConnectedToCentroidShapeLineStyleBase(const QString& name) : MyLineStyleBase(name) {

}

// MyConnectedToCentroidNodeLineStyle

MyConnectedToStartCentroidShapeLineStyle::MyConnectedToStartCentroidShapeLineStyle(const QString& name) : MyConnectedToCentroidShapeLineStyleBase(name) {

}

MyLineStyleBase::LINE_STYLE_TYPE MyConnectedToStartCentroidShapeLineStyle::lineType() {
    return CONNECTED_TO_START_CENTROID_SHAPE_LINE_STYLE;
}

// MyConnectedToEndCentroidShapeLineStyle

MyConnectedToEndCentroidShapeLineStyle::MyConnectedToEndCentroidShapeLineStyle(const QString& name) : MyConnectedToCentroidShapeLineStyleBase(name) {

}

MyLineStyleBase::LINE_STYLE_TYPE MyConnectedToEndCentroidShapeLineStyle::lineType() {
    return CONNECTED_TO_END_CENTROID_SHAPE_LINE_STYLE;
}