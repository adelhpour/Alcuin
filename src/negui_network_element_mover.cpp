#include "negui_network_element_mover.h"
#include "negui_node.h"

// MyNetworkElementMoverBase

MyNetworkElementMoverBase::MyNetworkElementMoverBase(QList<MyNetworkElementBase*> networkElements) {
    _networkElements = networkElements;
}

// MyNodeMover

MyNodeMover::MyNodeMover(QList<MyNetworkElementBase*> networkElements) : MyNetworkElementMoverBase(networkElements) {

}

void MyNodeMover::move(const qreal& dx, const qreal& dy) {
    for (MyNetworkElementBase* networkElement : _networkElements) {
        if (!whetherParentNodeIsInSelectedNetworkElements(networkElement))
            ((MyNodeBase*)networkElement)->move(dx, dy);
    }
}

const bool MyNodeMover::whetherParentNodeIsInSelectedNetworkElements(MyNetworkElementBase* networkElement) {
    return findElement(_networkElements, ((MyNodeBase*)networkElement)->parentNodeId());
}