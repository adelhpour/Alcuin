#ifndef __NEGUI_EDGE_STYLE_H
#define __NEGUI_EDGE_STYLE_H

#include "negui_element_style_base.h"

class MyEdgeStyleBase : public MyElementStyleBase {
public:

    MyEdgeStyleBase(const QString& name);
    
    const QString type() const override;
    
    bool isConnectableToNodeCategory(const QString& connectedNodeCategory, const QString& connectToNodeAs);
    
    bool isConnectableToSourceNodeCategory(const QString& connectedSourceNodeCategory);
    
    bool isConnectableToTargetNodeCategory(const QString& connectedRTargetNodeCategory);
    
    QList<QString> connectableSourceNodeCategories();
    
    QList<QString> connectableTargetNodeCategories();
    
    MyElementStyleBase* arrowHeadStyle();
    
    QObject* createIconBuilder() override;
    
    const QString toolTipText() override;
    
    const QString toolTipText(const QString& connectToNodeAs);
    
    const QString sourceToolTipText();
    
    const QString targetToolTipText();
    
    void read(const QJsonObject &json) override;
    
    void write(QJsonObject &json) override;

protected:
    MyElementStyleBase* _arrowHeadStyle;
    QList<QString> _connectableSourceNodeCategories;
    QList<QString> _connectableTargetNodeCategories;
};

class MyClassicEdgeStyle : public MyEdgeStyleBase {
public:

    MyClassicEdgeStyle(const QString& name);

    void addDefaultShapeStyle() override;

    MyShapeStyleBase* createShapeStyle(const QString& shape) override;
};

#endif
