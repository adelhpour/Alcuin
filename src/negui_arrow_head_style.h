#ifndef __NEGUI_ARROW_HEAD_STYLE_H
#define __NEGUI_ARROW_HEAD_STYLE_H

#include "negui_network_element_style_base.h"

class MyArrowHeadStyleBase : public MyNetworkElementStyleBase {
public:

    typedef enum {
        CLASSIC_ARROW_HEAD_STYLE,
    } ARROW_HEAD_STYLE_TYPE;

    MyArrowHeadStyleBase(const QString& name);
    
    const QString type() const override;

    virtual ARROW_HEAD_STYLE_TYPE arrowHeadStyleType() = 0;
    
    QObject* createIconBuilder() override;
    
    const QString toolTipText() override;
    
    void write(QJsonObject &json) override;
};

class MyClassicArrowHeadStyle : public MyArrowHeadStyleBase {
public:

    MyClassicArrowHeadStyle(const QString& name);

    ARROW_HEAD_STYLE_TYPE arrowHeadStyleType() override;

    void addDefaultShapeStyle() override;

    MyShapeStyleBase* createShapeStyle(const QString& shape) override;

    QWidget* shapeStylesButtons() override;
};

class MyChangeArrowHeadShapeStylesButton : public MyChangeShapeStylesButtonsBase {
    Q_OBJECT

public:

    MyChangeArrowHeadShapeStylesButton(QWidget* parent = nullptr);

    virtual void setMenu() override;
};

class MyAddRemoveArrowHeadShapeStylesButtons : public MyAddRemoveShapeStylesButtonsBase {
    Q_OBJECT
    
public:
    
    MyAddRemoveArrowHeadShapeStylesButtons(QWidget* parent = nullptr);
    
    virtual void setAddingMenu() override;
};

#endif
