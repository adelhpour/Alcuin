#ifndef __NEGUI_POLYGON_STYLE_H
#define __NEGUI_POLYGON_STYLE_H

#include "negui_shape_style_base.h"

class MyPolygonStyleBase : public MyShapeStyleBase {
public:
    
    MyPolygonStyleBase(const QString& name);
    
    SHAPE_STYLE type() override;
    
    // get the list of point parameters
    const QList<MyAbsolutePointParameter*> pointParameters() const;
    
    // get the list of points
    const QList<QPointF> points() const;
    
    // get the bounding rect of the polygon
    const QRectF boundingRect() const;
    
    // scale the points to fit the extents
    void scaleToExtents(const QRectF& extetns);
    
    // add the default points for a polygon style that contains no point
    virtual void addDefaultPoints() = 0;
    
    // reset all the values
    void reset() override;
    
    // read the node style info from the json object
    void read(const QJsonObject &json) override;
    
    // write the node style info to the json object
    void write(QJsonObject &json) override;
};

class MyNodePolygonStyle : public MyPolygonStyleBase {
public:
    
    MyNodePolygonStyle(const QString& name);
    
    // add the default points for a polygon style that contains no point
    void addDefaultPoints() override;
};

class MyArrowHeadPolygonStyle : public MyPolygonStyleBase {
public:
    
    MyArrowHeadPolygonStyle(const QString& name);
    
    // add the default points for a polygon style that contains no point
    void addDefaultPoints() override;
};

#endif
