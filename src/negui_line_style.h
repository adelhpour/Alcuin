#ifndef __NEGUI_LINE_STYLE_H
#define __NEGUI_LINE_STYLE_H

#include "negui_1d_shape_style_base.h"

class MyLineStyleBase : public My1DShapeStyleBase {
public:

    typedef enum {
        CLASSIC_LINE_STYLE,
        CONNECTED_TO_START_CENTROID_SHAPE_LINE_STYLE,
        CONNECTED_TO_END_CENTROID_SHAPE_LINE_STYLE,
    } LINE_STYLE_TYPE;
    
    MyLineStyleBase(const QString& name);
    
    SHAPE_STYLE type() override;

    virtual LINE_STYLE_TYPE lineType() = 0;
    
    const QRectF getShapeExtents() override;
    
    // set the relative value of p1
    void setRelativeP1(const QPointF& relativeP1) const;
    
    // get the relative value of p1
    const QPoint relativeP1() const;
    
    // set the relative value of p2
    void setRelativeP2(const QPointF& relativeP2) const;
    
    // get the relative value of p2
    const QPoint relativeP2() const;
    
    // read the node style info from the json object
    void read(const QJsonObject &json) override;
    
    // write the node style info to the json object
    void write(QJsonObject &json) override;
};

class MyClassicLineStyle : public MyLineStyleBase {
public:

    MyClassicLineStyle(const QString& name);

    LINE_STYLE_TYPE lineType() override;
};

class MyConnectedToCentroidShapeLineStyleBase : public MyLineStyleBase {
public:

    MyConnectedToCentroidShapeLineStyleBase(const QString& name);
};

class MyConnectedToStartCentroidShapeLineStyle : public MyConnectedToCentroidShapeLineStyleBase {
public:

    MyConnectedToStartCentroidShapeLineStyle(const QString& name);

    LINE_STYLE_TYPE lineType() override;
};

class MyConnectedToEndCentroidShapeLineStyle : public MyConnectedToCentroidShapeLineStyleBase {
public:

    MyConnectedToEndCentroidShapeLineStyle(const QString& name);

    LINE_STYLE_TYPE lineType() override;
};

#endif
