#ifndef __NEGUI_LINE_STYLE_H
#define __NEGUI_LINE_STYLE_H

#include "negui_shape_style_base.h"

class MyLineStyle : public MyShapeStyleBase {
public:
    
    MyLineStyle(const QString& name);
    
    SHAPE_STYLE type() override;
    
    // read the node style info from the json object
    void read(const QJsonObject &json) override;
    
    // write the node style info to the json object
    void write(QJsonObject &json) override;
};

class MyBezierStyle : public MyShapeStyleBase {
public:
    
    MyBezierStyle(const QString& name);
    
    SHAPE_STYLE type() override;
    
    // get the relative value of p1
    const QPoint relativeP1() const;
    
    // get the relative value of p2
    const QPoint relativeP2() const;
    
    // read the node style info from the json object
    void read(const QJsonObject &json) override;
    
    // write the node style info to the json object
    void write(QJsonObject &json) override;
};

#endif
