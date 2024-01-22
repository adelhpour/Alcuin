#ifndef __NEGUI_RECTANGLE_STYLE_H
#define __NEGUI_RECTANGLE_STYLE_H

#include "negui_2d_shape_style_base.h"

class MyRectangleStyleBase : public My2DShapeStyleBase {
    Q_OBJECT

public:

    MyRectangleStyleBase(const QString& name);

    SHAPE_STYLE type() override;

    const QRectF getShapeExtents() override;

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

    // set the x value of border radius
    void setBorderRadiusX(const qreal& borderRadiusX) const;

    // get the x value of border radius
    const qreal borderRadiusX() const;

    // set the y value of border radius
    void setBorderRadiusY(const qreal& borderRadiusY) const;

    // get the y value of border radius
    const qreal borderRadiusY() const;

    // read the node style info from the json object
    void read(const QJsonObject &json) override;

    // write the node style info to the json object
    void write(QJsonObject &json) override;

public slots:

    void updateShapeExtents(const QRectF& extents) override;
};

class MyNodeRectangleStyle : public MyRectangleStyleBase {
public:
    
    MyNodeRectangleStyle(const QString& name);
};

class MyArrowHeadRectangleStyle : public MyRectangleStyleBase {
public:
    
    MyArrowHeadRectangleStyle(const QString& name);
};

#endif
