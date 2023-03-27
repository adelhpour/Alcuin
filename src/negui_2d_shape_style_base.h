#ifndef __NEGUI_2D_SHAPE_STYLE_BASE_H
#define __NEGUI_2D_SHAPE_STYLE_BASE_H

#include "negui_1d_shape_style_base.h"

class My2DShapeStyleBase : public My1DShapeStyleBase {
public:

    My2DShapeStyleBase(const QString& name);

    virtual ~My2DShapeStyleBase();

    // get the brush for fill color
    const QBrush brush() const;

    // get the brush with selected fill color
    const QBrush selectedBrush() const;

    // read the node style info from the json object
    void read(const QJsonObject &json) override;

    // write the node style info to the json object
    void write(QJsonObject &json) override;
};

#endif
