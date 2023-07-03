#ifndef __NEGUI_EDGE_STYLE_H
#define __NEGUI_EDGE_STYLE_H

#include "negui_network_element_style_base.h"

class MyEdgeStyleBase : public MyNetworkElementStyleBase {
public:

    typedef enum {
        CLASSIC_EDGE_STYLE,
        CONNECTED_TO_CENTROID_NODE_EDGE_STYLE,
    } EDGE_STYLE_TYPE;

    MyEdgeStyleBase(const QString& name);
    
    const QString type() const override;

    virtual EDGE_STYLE_TYPE edgeStyleType() = 0;
    
    bool isConnectableToNodeCategory(const QString& connectedNodeCategory, const QString& connectToNodeAs);
    
    bool isConnectableToSourceNodeCategory(const QString& connectedSourceNodeCategory);
    
    bool isConnectableToTargetNodeCategory(const QString& connectedRTargetNodeCategory);
    
    QList<QString> connectableSourceNodeCategories();
    
    QList<QString> connectableTargetNodeCategories();
    
    MyNetworkElementStyleBase* arrowHeadStyle();
    
    QObject* createIconBuilder() override;
    
    const QString toolTipText() override;
    
    const QString toolTipText(const QString& connectToNodeAs);
    
    const QString sourceToolTipText();
    
    const QString targetToolTipText();
    
    void read(const QJsonObject &json) override;
    
    void write(QJsonObject &json) override;

protected:
    MyNetworkElementStyleBase* _arrowHeadStyle;
    QList<QString> _connectableSourceNodeCategories;
    QList<QString> _connectableTargetNodeCategories;
};

class MyClassicEdgeStyle : public MyEdgeStyleBase {
public:

    MyClassicEdgeStyle(const QString& name);

    EDGE_STYLE_TYPE edgeStyleType() override;

    void addDefaultShapeStyle() override;

    MyShapeStyleBase* createShapeStyle(const QString& shape) override;
};

class MyConnectedToCentroidNodeEdgeStyle : public MyEdgeStyleBase {
public:

    MyConnectedToCentroidNodeEdgeStyle(const QString& name);

    EDGE_STYLE_TYPE edgeStyleType() override;

    void addDefaultShapeStyle() override;

    MyShapeStyleBase* createShapeStyle(const QString& shape) override;
};

#endif
