#include "negui_network_element_aligner.h"
#include "negui_node.h"

// MyNetworkElementAlignerBase

MyNetworkElementAlignerBase::MyNetworkElementAlignerBase(QList<MyNetworkElementBase*> networkElements) {
    _networkElements = networkElements;
}

// MyNodeAligner

MyNodeAlignerBase::MyNodeAlignerBase(QList<MyNetworkElementBase*> networkElements) : MyNetworkElementAlignerBase(networkElements)  {

}

void MyNodeAlignerBase::align() {
    extractExtents();
    adjustNodePositions();
}

void MyNodeAlignerBase::extractExtents() {
    _minX = INT_MAX;
    _minY = INT_MAX;
    _maxX = INT_MIN;
    _maxY = INT_MIN;
    QPointF position;
    for (MyNetworkElementBase* node : _networkElements) {
            position = ((MyNodeBase*)node)->position();
            if (_minX > position.x())
                _minX = position.x();
            if (_maxX < position.x())
                _maxX = position.x();
            if (_minY > position.y())
                _minY = position.y();
            if (_maxY < position.y())
                _maxY = position.y();
    }
}

// MyNodeTopAligner

MyNodeTopAligner::MyNodeTopAligner(QList<MyNetworkElementBase*> networkElements) : MyNodeAlignerBase(networkElements) {

}

void MyNodeTopAligner::adjustNodePositions() {
    for (MyNetworkElementBase* node : _networkElements)
        ((MyNodeBase*)node)->graphicsItem()->moveBy(0, _minY - ((MyNodeBase*)node)->position().y());
}

// MyNodeMiddleAligner

MyNodeMiddleAligner::MyNodeMiddleAligner(QList<MyNetworkElementBase*> networkElements) : MyNodeAlignerBase(networkElements) {

}

void MyNodeMiddleAligner::adjustNodePositions() {
    for (MyNetworkElementBase* node : _networkElements)
        ((MyNodeBase*)node)->graphicsItem()->moveBy(0, 0.5 * (_minY + _maxY) - ((MyNodeBase*)node)->position().y());
}

// MyNodeBottomAligner

MyNodeBottomAligner::MyNodeBottomAligner(QList<MyNetworkElementBase*> networkElements) : MyNodeAlignerBase(networkElements) {

}

void MyNodeBottomAligner::adjustNodePositions() {
    for (MyNetworkElementBase* node : _networkElements)
        ((MyNodeBase*)node)->graphicsItem()->moveBy(0, _maxY - ((MyNodeBase*)node)->position().y());
}

// MyNodeLeftAligner

MyNodeLeftAligner::MyNodeLeftAligner(QList<MyNetworkElementBase*> networkElements) : MyNodeAlignerBase(networkElements) {

}

void MyNodeLeftAligner::adjustNodePositions() {
    for (MyNetworkElementBase* node : _networkElements)
        ((MyNodeBase*)node)->graphicsItem()->moveBy(_minX - ((MyNodeBase*)node)->position().x(), 0);
}

// MyNodeCenterAligner

MyNodeCenterAligner::MyNodeCenterAligner(QList<MyNetworkElementBase*> networkElements) : MyNodeAlignerBase(networkElements) {

}

void MyNodeCenterAligner::adjustNodePositions() {
    for (MyNetworkElementBase* node : _networkElements)
        ((MyNodeBase*)node)->graphicsItem()->moveBy(0.5 * (_minX + _maxX) - ((MyNodeBase*)node)->position().x(), 0);
}

// MyNodeRightAligner

MyNodeRightAligner::MyNodeRightAligner(QList<MyNetworkElementBase*> networkElements) : MyNodeAlignerBase(networkElements) {

}

void MyNodeRightAligner::adjustNodePositions() {
    for (MyNetworkElementBase* node : _networkElements)
        ((MyNodeBase*)node)->graphicsItem()->moveBy(_maxX - ((MyNodeBase*)node)->position().x(), 0);
}

// MyNodeDistributeAlignerBase

MyNodeDistributeAlignerBase::MyNodeDistributeAlignerBase(QList<MyNetworkElementBase*> networkElements) : MyNodeAlignerBase(networkElements) {

}

QList<MyNetworkElementBase*> MyNodeDistributeAlignerBase::getClassicNodes() {
    QList<MyNetworkElementBase*> classicNodes;
    for (MyNetworkElementBase* node : _networkElements) {
        if (((MyNodeBase*)node)->nodeType() != MyNodeBase::CENTROID_NODE)
            classicNodes.push_back(node);
    }

    return classicNodes;
}

// MyNodeHorizontallyDistributeAligner

MyNodeHorizontallyDistributeAligner::MyNodeHorizontallyDistributeAligner(QList<MyNetworkElementBase*> networkElements) : MyNodeDistributeAlignerBase(networkElements) {

}

void MyNodeHorizontallyDistributeAligner::adjustNodePositions() {
    QList<MyNetworkElementBase*> classicNodes = getClassicNodes();
    qreal distributionDistance = (_maxX - _minX) / (classicNodes.size() - 1);
    for (unsigned  int i = 0; i < classicNodes.size(); i++)
        ((MyNodeBase*)classicNodes.at(i))->graphicsItem()->moveBy(_minX - ((MyNodeBase*)classicNodes.at(i))->position().x() + i * distributionDistance, 0);
}

// MyNodeVerticallyDistributeAligner

MyNodeVerticallyDistributeAligner::MyNodeVerticallyDistributeAligner(QList<MyNetworkElementBase*> networkElements) : MyNodeDistributeAlignerBase(networkElements) {

}

void MyNodeVerticallyDistributeAligner::adjustNodePositions() {
    QList<MyNetworkElementBase*> classicNodes = getClassicNodes();
    qreal distributionDistance = (_maxY - _minY) / (classicNodes.size() - 1);
    for (unsigned  int i = 0; i < classicNodes.size(); i++)
        ((MyNodeBase*)classicNodes.at(i))->graphicsItem()->moveBy(0, _minY - ((MyNodeBase*)classicNodes.at(i))->position().y() + i * distributionDistance);
}
