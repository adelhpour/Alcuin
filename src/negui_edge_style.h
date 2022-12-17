#ifndef __NEGUI_EDGE_STYLE_H
#define __NEGUI_EDGE_STYLE_H

#include "negui_element_style_base.h"

class MyEdgeStyle : public MyElementStyleBase {
public:
    
    MyEdgeStyle(const QString& name);
    
    const QString type() const override;
    
    QList<QString> connectableStartNodeCategories();
    
    bool isConnectableToStartNodeCategory(const QString& connectedStartNodeCategory) override;
    
    QList<QString> connectableEndNodeCategories();
    
    bool isConnectableToEndNodeCategory(const QString& connectedEndNodeCategory) override;
    
    MyElementStyleBase* arrowHeadStyle();
    
    void addDefaultShapeStyle() override;
    
    MyShapeStyleBase* createShapeStyle(const QString& shape) override;
    
    QList<MyElementGraphicsItemBase*> getElementIconGraphicsItems() override;
    
    const QString toolTipText() override;
    
    const QString alternativeToolTipText() override;
    
    void read(const QJsonObject &json) override;
    
    void write(QJsonObject &json) override;

protected:
    MyElementStyleBase* _arrowHeadStyle;
    QList<QString> _connectableStartNodeCategories;
    QList<QString> _connectableEndNodeCategories;
};

class MyAddRemoveEdgeShapeStylesButtons : public MyAddRemoveShapeStylesButtonsBase {
    Q_OBJECT
    
public:
    
    MyAddRemoveEdgeShapeStylesButtons(QWidget* parent = nullptr);
    
    virtual void setAddingMenu() override;
};

#endif
