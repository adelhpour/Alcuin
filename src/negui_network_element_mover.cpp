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
    for (MyNetworkElementBase* networkElement : _networkElements)
        ((MyNodeBase*)networkElement)->moveExternally(dx, dy);
}