#include "negui_network_element_mover.h"
#include "negui_node.h"

// MyNetworkElementMoverBase

MyNetworkElementMoverBase::MyNetworkElementMoverBase(QList<MyNetworkElementBase*> networkElements, MyNetworkElementBase* movedNetworkElement) {
    _networkElements = networkElements;
    _movedNetworkElement = movedNetworkElement;
}

// MyNodeMover

MyNodeMover::MyNodeMover(QList<MyNetworkElementBase*> networkElements, MyNetworkElementBase* movedNetworkElement) : MyNetworkElementMoverBase(networkElements, movedNetworkElement) {

}

void MyNodeMover::move(const qreal& dx, const qreal& dy) {
    for (MyNetworkElementBase* networkElement : _networkElements) {
        if (networkElement != _movedNetworkElement)
            ((MyNodeBase*)networkElement)->moveExternally(dx, dy);
    }
}