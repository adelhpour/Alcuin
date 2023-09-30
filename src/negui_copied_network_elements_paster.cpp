#include "negui_copied_network_elements_paster.h"
#include "negui_network_element_base.h"
#include "negui_node.h"
#include "negui_edge.h"
#include "negui_node_builder.h"
#include "negui_edge_builder.h"
#include "negui_node_style_builder.h"
#include "negui_edge_style_builder.h"

// MyCopiedNetworkElementsPaster

MyCopiedNetworkElementsPaster::MyCopiedNetworkElementsPaster(QList<MyNetworkElementBase*> copiedNetworkElements, const QPointF& copyCenter) {
    _copiedNetworkElements = copiedNetworkElements;
    _copyCenter = copyCenter;
}

void MyCopiedNetworkElementsPaster::paste() {
    pasteNodes();
    pasteEdges();
}

void MyCopiedNetworkElementsPaster::pasteNodes() {
    QPointF movedCenter = _copyCenter - calculateElementsCenter();
    MyNetworkElementBase* pastedNode = NULL;
    for (MyNetworkElementBase* copiedElement: _copiedNetworkElements) {
        if (copiedElement->type() == MyNodeBase::NODE_ELEMENT) {
            pastedNode = createPastedNode(copiedElement, movedCenter);
            _nodeNamesHash[copiedElement->name()] = pastedNode->name();
            _pastedNodes.push_back(pastedNode);
            askForAddNode(pastedNode);
        }
    }
    updatePastedNodesParents();
}

MyNetworkElementBase* MyCopiedNetworkElementsPaster::createPastedNode(MyNetworkElementBase* node, const QPointF& movedCenter) {
    QString pastedNodeName = askForNodeUniqueName(node->style());
    return createNode(pastedNodeName, getCopyNodeStyle(pastedNodeName, node->style()), ((MyNodeBase*)node)->position().x() + movedCenter.x(), ((MyNodeBase*)node)->position().y() + movedCenter.y());
}

void MyCopiedNetworkElementsPaster::pasteEdges() {
    MyNetworkElementBase* pastedEdge = NULL;
    for (MyNetworkElementBase* copiedElement: _copiedNetworkElements) {
        if (copiedElement->type() == MyNodeBase::EDGE_ELEMENT) {
            pastedEdge = createPastedEdge(copiedElement);
            askForAddEdge(pastedEdge);
        }
    }
}

MyNetworkElementBase* MyCopiedNetworkElementsPaster::createPastedEdge(MyNetworkElementBase* edge) {
    QString pastedEdgeName = askForEdgeUniqueName(edge->style());
    return createEdge(pastedEdgeName, getCopyEdgeStyle(pastedEdgeName, edge->style()), findPastedNode(((MyEdgeBase*)edge)->sourceNode()->name()), findPastedNode(((MyEdgeBase*)edge)->targetNode()->name()));
}

void MyCopiedNetworkElementsPaster::updatePastedNodesParents() {
    MyNetworkElementBase* pastedNode = NULL;
    MyNetworkElementBase* pastedParentNode = NULL;
    for (MyNetworkElementBase* copiedElement: _copiedNetworkElements) {
        if (copiedElement->type() == MyNodeBase::NODE_ELEMENT) {
            pastedParentNode = findPastedNode(((MyNodeBase*)copiedElement)->parentNodeId());
            if (pastedParentNode) {
                pastedNode = findPastedNode(copiedElement->name());
                ((MyNodeBase*)pastedNode)->setParentNode(pastedParentNode);
            }
        }
    }
}

const QPointF MyCopiedNetworkElementsPaster::calculateElementsCenter() {
    QPointF elementsCenter(0.0, 0.0);
    qreal numberOfCopiedNodes = 0;
    for (MyNetworkElementBase* copiedElement : _copiedNetworkElements) {
        if (copiedElement->type() == MyNetworkElementBase::NODE_ELEMENT) {
            elementsCenter += ((MyNodeBase*)copiedElement)->position();
            ++ numberOfCopiedNodes;
        }
    }

    return elementsCenter / numberOfCopiedNodes;
}

MyNetworkElementBase* MyCopiedNetworkElementsPaster::findPastedNode(const QString& name) {
    for (MyNetworkElementBase* pastedNode : _pastedNodes)
        if (pastedNode->name() == _nodeNamesHash[name])
            return pastedNode;

    return NULL;
}

MyNetworkElementStyleBase* getCopyEdgeStyle(const QString& name, MyNetworkElementStyleBase* edgeStyle) {
    QJsonObject styleObject;
    edgeStyle->write(styleObject);
    styleObject["name"] = name;
    return createEdgeStyle(styleObject);
}

MyNetworkElementStyleBase* getCopyNodeStyle(const QString& name, MyNetworkElementStyleBase* nodeStyle) {
    QJsonObject styleObject;
    nodeStyle->write(styleObject);
    styleObject["name"] = name;
    return createNodeStyle(styleObject);
}
