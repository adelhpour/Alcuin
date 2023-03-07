#ifndef __NEGUI_ARROW_HEAD_STYLE_H
#define __NEGUI_ARROW_HEAD_STYLE_H

#include "negui_element_style_base.h"

class MyArrowHeadStyleBase : public MyElementStyleBase {
public:

    MyArrowHeadStyleBase(const QString& name);
    
    const QString type() const override;
    
    QObject* createIconBuilder() override;
    
    const QString toolTipText() override;
    
    void write(QJsonObject &json) override;
};

class MyClassicArrowHeadStyle : public MyArrowHeadStyleBase {
public:

    MyClassicArrowHeadStyle(const QString& name);

    void addDefaultShapeStyle() override;

    MyShapeStyleBase* createShapeStyle(const QString& shape) override;

    QWidget* getAddRemoveShapeStylesButtons();

protected:
    QWidget* _addRemoveShapeStylesButtons;
};

class MyAddRemoveArrowHeadShapeStylesButtons : public MyAddRemoveShapeStylesButtonsBase {
    Q_OBJECT
    
public:
    
    MyAddRemoveArrowHeadShapeStylesButtons(QWidget* parent = nullptr);
    
    virtual void setAddingMenu() override;
};

#endif
