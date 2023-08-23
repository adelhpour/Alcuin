#ifndef __NEGUI_1D_SHAPE_STYLE_BASE_H
#define __NEGUI_1D_SHAPE_STYLE_BASE_H

#include "negui_shape_style_base.h"

#include <QPen>

class My1DShapeStyleBase : public MyShapeStyleBase {
public:

    My1DShapeStyleBase(const QString& name);

    virtual ~My1DShapeStyleBase();

    // get the pen for the border line
    const QPen pen() const;

    // get the pen with selected border color
    const QPen selectedPen() const;

    // read the node style info from the json object
    void read(const QJsonObject &json) override;

    // write the node style info to the json object
    void write(QJsonObject &json) override;
};

#endif
