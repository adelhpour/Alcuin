#ifndef __NEGUI_RECT_STYLE_H
#define __NEGUI_RECT_STYLE_H

#include "negui_shape_style_base.h"

class MyRectStyleBase : public MyShapeStyleBase {
public:
    
    MyRectStyleBase(const QString& name);
    
    SHAPE_STYLE type() override;
    
    // set the x value of position
    void setX(const qreal& x) const;
    
    // get the x value of position
    const qreal x() const;
    
    // set the y value of position
    void setY(const qreal& y) const;
    
    // get the y value of position
    const qreal y() const;
    
    // set the value of width
    void setWidth(const qreal& width) const;
    
    // get the value of width
    const qreal width() const;
    
    // set the value of height
    void setHeight(const qreal& height) const;
    
    // get the value of height
    const qreal height() const;
    
    // set the x value of corner radius
    void setRx(const qreal& rx) const;
    
    // get the x value of corner radius
    const qreal rx() const;
    
    // set the y value of corner radius
    void setRy(const qreal& ry) const;
    
    // get the y value of corner radius
    const qreal ry() const;
    
    // read the node style info from the json object
    void read(const QJsonObject &json) override;
    
    // write the node style info to the json object
    void write(QJsonObject &json) override;
};

class MyNodeRectStyle : public MyRectStyleBase {
public:
    
    MyNodeRectStyle(const QString& name);
};

class MyArrowHeadRectStyle : public MyRectStyleBase {
public:
    
    MyArrowHeadRectStyle(const QString& name);
};

#endif
