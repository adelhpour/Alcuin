#include "negui_line_style.h"

// MyLineStyle

MyLineStyle::MyLineStyle(const QString& name) : MyShapeStyleBase(name) {
    // stroke-width
    _parameters.push_back(new MyStrokeWidthParameter());
    
    // stroke
    _parameters.push_back(new MyStrokeParameter());
    
    reset();
}

MyShapeStyleBase::SHAPE_STYLE MyLineStyle::type() {
    return LINE_SHAPE_STYLE;
}

const QRectF MyLineStyle::getShapeExtents() {
    return QRectF(0.0, 0.0, 0.0, 0.0);
}

void MyLineStyle::read(const QJsonObject &json) {
    MyShapeStyleBase::read(json);
}

void MyLineStyle::write(QJsonObject &json) {
    MyShapeStyleBase::write(json);
}
