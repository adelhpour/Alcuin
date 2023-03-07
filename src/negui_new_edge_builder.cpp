#include "negui_new_edge_builder.h"
#include "negui_element_builder.h"
#include "negui_node.h"
#include "negui_edge.h"
#include "negui_element_style_builder.h"
#include "negui_edge_style.h"
#include "negui_template_style.h"

// MyNewEdgeBuilderBase

MyNewEdgeBuilderBase::MyNewEdgeBuilderBase(MyElementStyleBase* style) {
    _style = style;
    _isNewEdgeBuilt = false;
}

void MyNewEdgeBuilderBase::build(MyElementBase* node) {
    if (canSelectNodeAsConnectedNode(node)) {
        if (canBuildEdgeUsingSelcetedNodes()) {
            buildNewEdge();
            _isNewEdgeBuilt = true;
        }
    }
}

const bool MyNewEdgeBuilderBase::canSelectNodeAsConnectedNode(MyElementBase* node) {
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

bool MyNewEdgeBuilderBase::isConnectableToNode(MyElementBase* node) {
    return ((MyEdgeStyleBase*)edgeStyle())->isConnectableToNodeCategory(node->style()->category(), connectToNodeAs()) ? true : false;
}

const bool MyNewEdgeBuilderBase::canBuildEdgeUsingSelcetedNodes() {
    if (selectedEdgeSourceNodes().size() + selectedEdgeTargetNodes().size() == numberOfRequiredSourceNodes() + numberOfRequiredTargetNodes())
        return true;
    
    return false;
}

void MyNewEdgeBuilderBase::buildNewEdge(MyElementBase* sourceNode, MyElementBase* targetNode, MyElementStyleBase* newEdgeStyle) {
    MyElementBase* newEdge = createEdge(askForEdgeUniqueName(newEdgeStyle), getCopyEdgeStyle(style()->name() + "_" + newEdgeStyle->name(), newEdgeStyle), sourceNode, targetNode);
    emit askForAddEdge(newEdge);
}

void MyNewEdgeBuilderBase::addToselectedEdgeSourceNodes(MyElementBase* selectedSourceNode) {
    _selectedEdgeSourceNodes.push_back(selectedSourceNode);
}

QList<MyElementBase*> MyNewEdgeBuilderBase::selectedEdgeSourceNodes() {
    return _selectedEdgeSourceNodes;
}

void MyNewEdgeBuilderBase::addToselectedEdgeTargetNodes(MyElementBase* selectedTargetNode) {
    _selectedEdgeTargetNodes.push_back(selectedTargetNode);
}

QList<MyElementBase*> MyNewEdgeBuilderBase::selectedEdgeTargetNodes() {
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
    for (MyElementBase* selectedEdgeSourceNode : selectedEdgeSourceNodes())
        MyNewEdgeBuilderBase::buildNewEdge(selectedEdgeSourceNode, intermediaryNode(), edgeStyle(selectedEdgeSourceNode));
    for (MyElementBase* selectedEdgeTargetNode : selectedEdgeTargetNodes())
        MyNewEdgeBuilderBase::buildNewEdge(intermediaryNode(), selectedEdgeTargetNode, edgeStyle(selectedEdgeTargetNode));
}

void MyNewTemplateBuilder::buildIntermediaryNode() {
    if (((MyTemplateStyle*)style())->intermediaryNodeStyle()) {
        QPointF position = intermediaryNodePosition();
        _intermediaryNode = createNode(askForNodeUniqueName(((MyTemplateStyle*)style())->intermediaryNodeStyle()), getCopyNodeStyle(style()->name() + "_" + ((MyTemplateStyle*)style())->intermediaryNodeStyle()->name(), ((MyTemplateStyle*)style())->intermediaryNodeStyle()), position.x(), position.y());
        askForAddNode(_intermediaryNode);
    }
}

MyElementBase* MyNewTemplateBuilder::intermediaryNode() {
    return _intermediaryNode;
}

const QPointF MyNewTemplateBuilder::intermediaryNodePosition() {
    QPointF position(0.0, 0.0);
    for (MyElementBase* connectedNode : selectedEdgeSourceNodes() + selectedEdgeTargetNodes())
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

MyElementStyleBase* MyNewTemplateBuilder::edgeStyle(MyElementBase* selectedNode) {
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
