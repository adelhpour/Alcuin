#ifndef __NEGUI_CENTROID_STYLE_H
#define __NEGUI_CENTROID_STYLE_H

#include "negui_2d_shape_style_base.h"

class MyCentroidStyleBase : public My2DShapeStyleBase {
    Q_OBJECT

public:

    MyCentroidStyleBase(const QString& name);

    SHAPE_STYLE type() override;

    const QRectF getShapeExtents() override;

    // set the value of radius
    void setRadius(const qreal& rx) const;

    // get the value of radius
    const qreal radius() const;

    void updateShapeExtents(const QRectF& extents) override;

    // read the node style info from the json object
    void read(const QJsonObject &json) override;

    // write the node style info to the json object
    void write(QJsonObject &json) override;
};

class MyNodeCentroidStyle : public MyCentroidStyleBase {
public:
    
    MyNodeCentroidStyle(const QString& name);
};

#endif
