#include "negui_new_edge_builder.h"
#include "negui_node.h"
#include "negui_edge.h"
#include "negui_node_builder.h"
#include "negui_edge_builder.h"
#include "negui_edge_style.h"
#include "negui_template_style.h"

// MyNewEdgeBuilderBase

MyNewEdgeBuilderBase::MyNewEdgeBuilderBase(MyElementStyleBase* style) {
    _style = style;
    _isNewEdgeBuilt = false;
}

void MyNewEdgeBuilderBase::build(MyNetworkElementBase* node) {
    if (canSelectNodeAsConnectedNode(node)) {
        if (canBuildEdgeUsingSelectedNodes()) {
            buildNewEdge();
            _isNewEdgeBuilt = true;
        }
    }
}

const bool MyNewEdgeBuilderBase::canSelectNodeAsConnectedNode(MyNetworkElementBase* node) {
    if (!node->isSelected() && isConnectableToNode(node)) {
        node->setSelected(true);
        if (selectedEdgeSourceNodes().size() < numberOfRequiredSourceNodes())
            addToselectedEdgeSourceNodes(node);
        else if (selectedEdgeTargetNodes().size() < numberOfRequiredTargetNodes())
            addToselectedEdgeTargetNodes(node);
        return true;
    }
        
    return false;
}

bool MyNewEdgeBuilderBase::isConnectableToNode(MyNetworkElementBase* node) {
    return ((MyEdgeStyleBase*)edgeStyle())->isConnectableToNodeCategory(node->style()->category(), connectToNodeAs()) ? true : false;
}

const bool MyNewEdgeBuilderBase::canBuildEdgeUsingSelectedNodes() {
    if (selectedEdgeSourceNodes().size() + selectedEdgeTargetNodes().size() == numberOfRequiredSourceNodes() + numberOfRequiredTargetNodes())
        return true;
    
    return false;
}

void MyNewEdgeBuilderBase::buildNewEdge(MyNetworkElementBase* sourceNode, MyNetworkElementBase* targetNode, MyElementStyleBase* newEdgeStyle) {
    MyNetworkElementBase* newEdge = createEdge(askForEdgeUniqueName(newEdgeStyle), getCopyEdgeStyle(style()->name() + "_" + newEdgeStyle->name(), newEdgeStyle), sourceNode, targetNode);
    emit askForAddEdge(newEdge);
}

void MyNewEdgeBuilderBase::addToselectedEdgeSourceNodes(MyNetworkElementBase* selectedSourceNode) {
    _selectedEdgeSourceNodes.push_back(selectedSourceNode);
}

QList<MyNetworkElementBase*> MyNewEdgeBuilderBase::selectedEdgeSourceNodes() {
    return _selectedEdgeSourceNodes;
}

void MyNewEdgeBuilderBase::addToselectedEdgeTargetNodes(MyNetworkElementBase* selectedTargetNode) {
    _selectedEdgeTargetNodes.push_back(selectedTargetNode);
}

QList<MyNetworkElementBase*> MyNewEdgeBuilderBase::selectedEdgeTargetNodes() {
    return _selectedEdgeTargetNodes;
}

const QString MyNewEdgeBuilderBase::toolTipText() {
    return ((MyEdgeStyleBase*)edgeStyle())->toolTipText(connectToNodeAs());
}

const QString MyNewEdgeBuilderBase::connectToNodeAs() {
    if (selectedEdgeSourceNodes().size() == numberOfRequiredSourceNodes() && selectedEdgeTargetNodes().size() < numberOfRequiredTargetNodes())
        return "target";
    
    return "source";
}

MyElementStyleBase* MyNewEdgeBuilderBase::style() {
    return _style;
}

// MyNewEdgeBuilder

MyNewEdgeBuilder::MyNewEdgeBuilder(MyElementStyleBase* edgeStyle) : MyNewEdgeBuilderBase(edgeStyle) {
    
}

void MyNewEdgeBuilder::buildNewEdge() {
    MyNewEdgeBuilderBase::buildNewEdge(selectedEdgeSourceNodes().at(0), selectedEdgeTargetNodes().at(0), edgeStyle());
}

MyElementStyleBase* MyNewEdgeBuilder::edgeStyle() {
    return style();
}

const qint32 MyNewEdgeBuilder::numberOfRequiredSourceNodes() {
    return 1;
}

const qint32 MyNewEdgeBuilder::numberOfRequiredTargetNodes() {
    return 1;
}

// MyNewTemplateBuilder

MyNewTemplateBuilder::MyNewTemplateBuilder(MyElementStyleBase* templateStyle) : MyNewEdgeBuilderBase(templateStyle) {
    _intermediaryNode = NULL;
}

void MyNewTemplateBuilder::buildNewEdge() {
    buildIntermediaryNode();
    for (MyNetworkElementBase* selectedEdgeSourceNode : selectedEdgeSourceNodes())
        MyNewEdgeBuilderBase::buildNewEdge(selectedEdgeSourceNode, intermediaryNode(), edgeStyle(selectedEdgeSourceNode));
    for (MyNetworkElementBase* selectedEdgeTargetNode : selectedEdgeTargetNodes())
        MyNewEdgeBuilderBase::buildNewEdge(intermediaryNode(), selectedEdgeTargetNode, edgeStyle(selectedEdgeTargetNode));
    setIntermediaryNodeParent();
}

void MyNewTemplateBuilder::buildIntermediaryNode() {
    if (((MyTemplateStyle*)style())->intermediaryNodeStyle()) {
        QPointF position = intermediaryNodePosition();
        _intermediaryNode = createNode(askForNodeUniqueName(((MyTemplateStyle*)style())->intermediaryNodeStyle()), getCopyNodeStyle(style()->name() + "_" + ((MyTemplateStyle*)style())->intermediaryNodeStyle()->name(), ((MyTemplateStyle*)style())->intermediaryNodeStyle()), position.x(), position.y());
        askForAddNode(_intermediaryNode);
    }
}

void MyNewTemplateBuilder::setIntermediaryNodeParent() {
    MyNetworkElementBase* parentNode = NULL;
    for (MyNetworkElementBase* selectedEdgeSourceNode : selectedEdgeSourceNodes() + selectedEdgeTargetNodes()) {
        if (!((MyNodeBase*)selectedEdgeSourceNode)->isSetParentNode())
            return;
        if (!parentNode)
            parentNode = ((MyNodeBase*)selectedEdgeSourceNode)->parentNode();
        else {
            if (((MyNodeBase*)selectedEdgeSourceNode)->parentNodeId() != parentNode->name())
                return;
        }
    }

    if (parentNode != NULL)
        ((MyNodeBase*)_intermediaryNode)->setParentNode(parentNode);
}

MyNetworkElementBase* MyNewTemplateBuilder::intermediaryNode() {
    return _intermediaryNode;
}

const QPointF MyNewTemplateBuilder::intermediaryNodePosition() {
    QPointF position(0.0, 0.0);
    for (MyNetworkElementBase* connectedNode : selectedEdgeSourceNodes() + selectedEdgeTargetNodes())
        position += ((MyNodeBase*)connectedNode)->position();
    position /= selectedEdgeSourceNodes().size() + selectedEdgeTargetNodes().size();
    
    return position;
}

MyElementStyleBase* MyNewTemplateBuilder::edgeStyle() {
    if (selectedEdgeSourceNodes().size() < numberOfRequiredSourceNodes())
        return ((MyTemplateStyle*)style())->sourceEdgeStyles().at(selectedEdgeSourceNodes().size());
    else if (selectedEdgeTargetNodes().size() < numberOfRequiredTargetNodes())
        return ((MyTemplateStyle*)style())->targetEdgeStyles().at(selectedEdgeTargetNodes().size());
    
    return ((MyTemplateStyle*)style())->sourceEdgeStyles().at(0);
}

MyElementStyleBase* MyNewTemplateBuilder::edgeStyle(MyNetworkElementBase* selectedNode) {
    if (selectedEdgeSourceNodes().indexOf(selectedNode) != -1)
        return ((MyTemplateStyle*)style())->sourceEdgeStyles().at(selectedEdgeSourceNodes().indexOf(selectedNode));
    else if (selectedEdgeTargetNodes().indexOf(selectedNode) != -1)
        return ((MyTemplateStyle*)style())->targetEdgeStyles().at(selectedEdgeTargetNodes().indexOf(selectedNode));
    
    return ((MyTemplateStyle*)style())->sourceEdgeStyles().at(0);;
}

const qint32 MyNewTemplateBuilder::numberOfRequiredSourceNodes() {
    return ((MyTemplateStyle*)style())->sourceEdgeStyles().size();
}

const qint32 MyNewTemplateBuilder::numberOfRequiredTargetNodes() {
    return ((MyTemplateStyle*)style())->targetEdgeStyles().size();
}
