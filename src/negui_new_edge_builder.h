#ifndef __NEGUI_NEW_EDGE_BUILDER_H
#define __NEGUI_NEW_EDGE_BUILDER_H

#include "negui_network_element_style_base.h"
#include "negui_network_element_base.h"
#include <QObject>

class MyNewEdgeBuilderBase : public QObject {
    Q_OBJECT
    
public:
    MyNewEdgeBuilderBase(MyNetworkElementStyleBase* style);
    
    void build(MyNetworkElementBase* node);
    
    virtual void buildNewEdge() = 0;
    
    const bool canSelectNodeAsConnectedNode(MyNetworkElementBase* node);
    
    bool isConnectableToNode(MyNetworkElementBase* node);
    
    const bool canBuildEdgeUsingSelectedNodes();
    
    void buildNewEdge(MyNetworkElementBase* sourceNode, MyNetworkElementBase* targetNode, MyNetworkElementStyleBase* newEdgeStyle);
    
    void addToselectedEdgeSourceNodes(MyNetworkElementBase* selectedSourceNode);
    
    QList<MyNetworkElementBase*> selectedEdgeSourceNodes();
    
    void addToselectedEdgeTargetNodes(MyNetworkElementBase* selectedTargetNode);
    
    QList<MyNetworkElementBase*> selectedEdgeTargetNodes();
    
    const QString toolTipText();
    
    const QString connectToNodeAs();
    
    MyNetworkElementStyleBase* style();
    
    virtual MyNetworkElementStyleBase* edgeStyle() = 0;
    
    virtual const qint32 numberOfRequiredSourceNodes() = 0;
    
    virtual const qint32 numberOfRequiredTargetNodes() = 0;
    
    const bool isNewEdgeBuilt() { return _isNewEdgeBuilt; }
    
signals:
    void askForAddEdge(MyNetworkElementBase*);
    QString askForEdgeUniqueName(MyNetworkElementStyleBase*);
    
protected:
    MyNetworkElementStyleBase* _style;
    QList<MyNetworkElementBase*> _selectedEdgeSourceNodes;
    QList<MyNetworkElementBase*> _selectedEdgeTargetNodes;
    bool _isNewEdgeBuilt;
};

class MyNewEdgeBuilder : public MyNewEdgeBuilderBase {
    
public:
    MyNewEdgeBuilder(MyNetworkElementStyleBase* edgeStyle);
    
    void buildNewEdge() override;
    
    MyNetworkElementStyleBase* edgeStyle() override;
    
    const qint32 numberOfRequiredSourceNodes() override;
    
    const qint32 numberOfRequiredTargetNodes() override;
};

class MyNewTemplateBuilder : public MyNewEdgeBuilderBase {
    Q_OBJECT
    
public:
    MyNewTemplateBuilder(MyNetworkElementStyleBase* templateStyle);
    
    void buildNewEdge() override;
    
    void buildIntermediaryNode();

    void setIntermediaryNodeParent();
    
    MyNetworkElementBase* intermediaryNode();
    
    const QPointF intermediaryNodePosition();
    
    MyNetworkElementStyleBase* edgeStyle() override;
    
    MyNetworkElementStyleBase* edgeStyle(MyNetworkElementBase* selectedNode);
    
    const qint32 numberOfRequiredSourceNodes() override;
    
    const qint32 numberOfRequiredTargetNodes() override;
    
signals:
    void askForAddNode(MyNetworkElementBase*);
    QString askForNodeUniqueName(MyNetworkElementStyleBase*);
    
protected:
    MyNetworkElementBase* _intermediaryNode;
};

MyNetworkElementStyleBase* getCopyNodeStyle(const QString& name, MyNetworkElementStyleBase* nodeStyle);
MyNetworkElementStyleBase* getCopyEdgeStyle(const QString& name, MyNetworkElementStyleBase* edgeStyle);

#endif
