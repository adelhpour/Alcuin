#ifndef __NEGUI_TEXT_STYLE_H
#define __NEGUI_TEXT_STYLE_H

#include "negui_2d_shape_style_base.h"

class MyTextStyleBase : public My2DShapeStyleBase {
public:
    
    MyTextStyleBase(const QString& name);
    
    SHAPE_STYLE type() override;

    // get the plain text for text
    virtual const QString plainText() const = 0;

    // set the plain text for text
    virtual void setPlainText(const QString& plainText) = 0;
    
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

    // set the associated name as the default text
    const bool& whetherSetNameAsDefaultPlainText() const;
    
    // read the node style info from the json object
    void read(const QJsonObject &json) override;
    
    // write the node style info to the json object
    void write(QJsonObject &json) override;

protected:

    bool _whetherSetNameAsDefaultPlainText;
};

class MySimpleTextStyle : public MyTextStyleBase {
public:

    MySimpleTextStyle(const QString& name);

    // get the plain text for text
    const QString plainText() const override;

    // set the plain text for text
    void setPlainText(const QString& plainText) override;

protected:

    QString _plainText;
};

class MyWithPlainTextTextStyle : public MyTextStyleBase {
public:

    MyWithPlainTextTextStyle(const QString& name);

    // get the plain text for text
    const QString plainText() const override;

    // set the plain text for text
    void setPlainText(const QString& plainText) override;

    // read the node style info from the json object
    void read(const QJsonObject &json) override;

    // write the node style info to the json object
    void write(QJsonObject &json) override;
};

#endif
