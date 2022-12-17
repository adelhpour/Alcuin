#ifndef __NEGUI_ARROW_HEAD_STYLE_H
#define __NEGUI_ARROW_HEAD_STYLE_H

#include "negui_element_style_base.h"

class MyArrowHeadStyle : public MyElementStyleBase {
public:
    
    MyArrowHeadStyle(const QString& name);
    
    const QString type() const override;
    
    void addDefaultShapeStyle() override;
    
    MyShapeStyleBase* createShapeStyle(const QString& shape) override;
    
    QList<MyElementGraphicsItemBase*> getElementIconGraphicsItems() override;
    
    const QString toolTipText() override;
    
    const QString alternativeToolTipText() override;
    
    void write(QJsonObject &json) override;
};

class MyAddRemoveArrowHeadShapeStylesButtons : public MyAddRemoveShapeStylesButtonsBase {
    Q_OBJECT
    
public:
    
    MyAddRemoveArrowHeadShapeStylesButtons(QWidget* parent = nullptr);
    
    virtual void setAddingMenu() override;
};

#endif
