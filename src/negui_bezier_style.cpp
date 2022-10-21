#include "negui_bezier_style.h"
#include <QJsonObject>

// MyBezierStyle

MyBezierStyle::MyBezierStyle(const QString& name) : MyShapeStyleBase(name) {
    // stroke-width
    _parameters.push_back(new MyStrokeWidthParameter());
    
    // stroke
    _parameters.push_back(new MyStrokeParameter());
    
    // control point 1
    _parameters.push_back(new MyRelativePointParameter("ControlPoint1"));
    
    // control point 2
    _parameters.push_back(new MyRelativePointParameter("ControlPoint2"));
    
    reset();
}

MyShapeStyleBase::SHAPE_STYLE MyBezierStyle::type() {
    return BEZIER_SHAPE_STYLE;
}

const QPoint MyBezierStyle::relativeP1() const {
    QPoint point;
    MyParameterBase* parameter = findParameter("ControlPoint1");
    if (parameter)
        point = QPoint(((MyRelativePointParameter*)parameter)->defaultValueX(), ((MyRelativePointParameter*)parameter)->defaultValueY());
    
    return point;
}

const QPoint MyBezierStyle::relativeP2() const {
    QPoint point;
    MyParameterBase* parameter = findParameter("ControlPoint2");
    if (parameter)
        point = QPoint(((MyRelativePointParameter*)parameter)->defaultValueX(), ((MyRelativePointParameter*)parameter)->defaultValueY());
    
    return point;
}

void MyBezierStyle::read(const QJsonObject &json) {
    MyShapeStyleBase::read(json);
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

void MyBezierStyle::write(QJsonObject &json) {
    MyShapeStyleBase::write(json);
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
