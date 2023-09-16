#ifndef __NEGUI_TEXT_STYLE_H
#define __NEGUI_TEXT_STYLE_H

#include "negui_2d_shape_style_base.h"

class MyTextStyleBase : public My2DShapeStyleBase {
    Q_OBJECT

public:

    MyTextStyleBase(const QString& name);

    // get the plain text for text
    const QString plainText() const;

    // set the plain text for text
    void setPlainText(const QString& plainText);

    const QRectF getShapeExtents() override;

    // get the default color for text
    const QColor defaultTextColor() const;

    // get the font for text
    const QFont font() const;

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

    // get the value of text height
    const qreal height() const;

    // get the horizontal alignment of text
    const Qt::Alignment horizontalAlignment() const;

    // get the vertical alignment of text
    const Qt::Alignment verticalAlignment() const;

    const qreal verticalPadding() const;

    const qreal calculateTopPaddingRatioToFontHeight(const qreal& pointSize) const ;

    // read the node style info from the json object
    void read(const QJsonObject &json) override;

    // write the node style info to the json object
    void write(QJsonObject &json) override;

public slots:

    void updateShapeExtents(const QRectF& extents) override;
};

class MySimpleTextStyle : public MyTextStyleBase {
public:

    MySimpleTextStyle(const QString& name);

    SHAPE_STYLE type() override;

    const bool& whetherSetNameAsDefaultPlainText() const;

    // read the node style info from the json object
    void read(const QJsonObject &json) override;

    // write the node style info to the json object
    void write(QJsonObject &json) override;

protected:

    bool _whetherSetNameAsDefaultPlainText;
};

class MyWithPlainTextTextStyle : public MyTextStyleBase {
public:

    MyWithPlainTextTextStyle(const QString& name);

    SHAPE_STYLE type() override;
};

#endif
