#ifndef __NEGUI_EDGE_STYLE_H
#define __NEGUI_EDGE_STYLE_H

#include "negui_network_element_style_base.h"

class MyEdgeStyleBase : public MyNetworkElementStyleBase {
public:

    typedef enum {
        CLASSIC_EDGE_STYLE,
        CONNECTED_TO_SOURCE_CENTROID_NODE_EDGE_STYLE,
        CONNECTED_TO_TARGET_CENTROID_NODE_EDGE_STYLE,
    } EDGE_STYLE_TYPE;

    MyEdgeStyleBase(const QString& name);
    
    const QString type() const override;

    virtual EDGE_STYLE_TYPE edgeStyleType() = 0;
    
    bool isConnectableToNodeCategory(const QString& connectedNodeCategory, const QString& connectToNodeAs);
    
    bool isConnectableToSourceNodeCategory(const QString& connectedSourceNodeCategory);
    
    bool isConnectableToTargetNodeCategory(const QString& connectedRTargetNodeCategory);

    const QString& connectableSourceNodeTitle();
    
    QList<QString> connectableSourceNodeCategories();

    const QString& connectableTargetNodeTitle();
    
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
    QString _connectableSourceNodeTitle;
    QList<QString> _connectableSourceNodeCategories;
    QString _connectableTargetNodeTitle;
    QList<QString> _connectableTargetNodeCategories;
};

class MyClassicEdgeStyle : public MyEdgeStyleBase {
public:

    MyClassicEdgeStyle(const QString& name);

    EDGE_STYLE_TYPE edgeStyleType() override;

    void addDefaultShapeStyle() override;

    MyShapeStyleBase* createShapeStyle(const QString& shape) override;
};

class MyConnectedToCentroidNodeEdgeStyleBase : public MyEdgeStyleBase {
public:

    MyConnectedToCentroidNodeEdgeStyleBase(const QString& name);
};

class MyConnectedToSourceCentroidNodeEdgeStyle : public MyConnectedToCentroidNodeEdgeStyleBase {

public:

    MyConnectedToSourceCentroidNodeEdgeStyle(const QString& name);

    EDGE_STYLE_TYPE edgeStyleType() override;

    void addDefaultShapeStyle() override;

    MyShapeStyleBase* createShapeStyle(const QString& shape) override;
};

class MyConnectedToTargetCentroidNodeEdgeStyle : public MyConnectedToCentroidNodeEdgeStyleBase {
public:

    MyConnectedToTargetCentroidNodeEdgeStyle(const QString& name);

    EDGE_STYLE_TYPE edgeStyleType() override;

    void addDefaultShapeStyle() override;

    MyShapeStyleBase* createShapeStyle(const QString& shape) override;
};

#endif
