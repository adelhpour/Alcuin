#ifndef __NEGUI_ELLIPSE_STYLE_H
#define __NEGUI_ELLIPSE_STYLE_H

#include "negui_2d_shape_style_base.h"

class MyEllipseStyleBase : public My2DShapeStyleBase {
    Q_OBJECT

public:
    
    MyEllipseStyleBase(const QString& name);
    
    SHAPE_STYLE type() override;
    
    const QRectF getShapeExtents() override;
    
    // set the cx value of center position
    void setCx(const qreal& cx) const;
    
    // get the x value of center position
    const qreal cx() const;
    
    // set the cy value of center position
    void setCy(const qreal& cy) const;
    
    // get the y value of center position
    const qreal cy() const;
    
    // set the value of horizontal radius
    void setRx(const qreal& rx) const;
    
    // get the value of horizontal radius
    const qreal rx() const;
    
    // set the value of vertical radius
    void setRy(const qreal& rx) const;
    
    // get the value of vertical radius
    const qreal ry() const;
    
    // read the node style info from the json object
    void read(const QJsonObject &json) override;
    
    // write the node style info to the json object
    void write(QJsonObject &json) override;

public slots:

    void updateShapeExtents(const QRectF& extents) override;
};

class MyNodeEllipseStyle : public MyEllipseStyleBase {
public:
    
    MyNodeEllipseStyle(const QString& name);
};

class MyArrowHeadEllipseStyle : public MyEllipseStyleBase {
public:
    
    MyArrowHeadEllipseStyle(const QString& name);
};

#endif
