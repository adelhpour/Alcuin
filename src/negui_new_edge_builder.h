#ifndef __NEGUI_NEW_EDGE_BUILDER_H
#define __NEGUI_NEW_EDGE_BUILDER_H

#include "negui_element_style_base.h"
#include "negui_element_base.h"
#include <QObject>

class MyNewEdgeBuilderBase : public QObject {
    Q_OBJECT
    
public:
    MyNewEdgeBuilderBase(MyElementStyleBase* style);
    
    void build(MyElementBase* node);
    
    virtual void buildNewEdge() = 0;
    
    const bool canSelectNodeAsConnectedNode(MyElementBase* node);
    
    bool isConnectableToNode(MyElementBase* node);
    
    const bool canBuildEdgeUsingSelectedNodes();
    
    void buildNewEdge(MyElementBase* sourceNode, MyElementBase* targetNode, MyElementStyleBase* newEdgeStyle);
    
    void addToselectedEdgeSourceNodes(MyElementBase* selectedSourceNode);
    
    QList<MyElementBase*> selectedEdgeSourceNodes();
    
    void addToselectedEdgeTargetNodes(MyElementBase* selectedTargetNode);
    
    QList<MyElementBase*> selectedEdgeTargetNodes();
    
    const QString toolTipText();
    
    const QString connectToNodeAs();
    
    MyElementStyleBase* style();
    
    virtual MyElementStyleBase* edgeStyle() = 0;
    
    virtual const qint32 numberOfRequiredSourceNodes() = 0;
    
    virtual const qint32 numberOfRequiredTargetNodes() = 0;
    
    const bool isNewEdgeBuilt() { return _isNewEdgeBuilt; }
    
signals:
    void askForAddEdge(MyElementBase*);
    QString askForEdgeUniqueName(MyElementStyleBase*);
    
protected:
    MyElementStyleBase* _style;
    QList<MyElementBase*> _selectedEdgeSourceNodes;
    QList<MyElementBase*> _selectedEdgeTargetNodes;
    bool _isNewEdgeBuilt;
};

class MyNewEdgeBuilder : public MyNewEdgeBuilderBase {
    
public:
    MyNewEdgeBuilder(MyElementStyleBase* edgeStyle);
    
    void buildNewEdge() override;
    
    MyElementStyleBase* edgeStyle() override;
    
    const qint32 numberOfRequiredSourceNodes() override;
    
    const qint32 numberOfRequiredTargetNodes() override;
};

class MyNewTemplateBuilder : public MyNewEdgeBuilderBase {
    Q_OBJECT
    
public:
    MyNewTemplateBuilder(MyElementStyleBase* templateStyle);
    
    void buildNewEdge() override;
    
    void buildIntermediaryNode();

    void setIntermediaryNodeParent();
    
    MyElementBase* intermediaryNode();
    
    const QPointF intermediaryNodePosition();
    
    MyElementStyleBase* edgeStyle() override;
    
    MyElementStyleBase* edgeStyle(MyElementBase* selectedNode);
    
    const qint32 numberOfRequiredSourceNodes() override;
    
    const qint32 numberOfRequiredTargetNodes() override;
    
signals:
    void askForAddNode(MyElementBase*);
    QString askForNodeUniqueName(MyElementStyleBase*);
    
protected:
    MyElementBase* _intermediaryNode;
};

MyElementStyleBase* getCopyNodeStyle(const QString& name, MyElementStyleBase* nodeStyle);
MyElementStyleBase* getCopyEdgeStyle(const QString& name, MyElementStyleBase* edgeStyle);

#endif
