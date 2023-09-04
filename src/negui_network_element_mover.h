#ifndef __NEGUI_NETWORK_ELEMENT_MOVER_H
#define __NEGUI_NETWORK_ELEMENT_MOVER_H

#include "negui_network_element_base.h"

class MyNetworkElementMoverBase : public QObject {
    Q_OBJECT

public:

    MyNetworkElementMoverBase(QList<MyNetworkElementBase*> networkElements, MyNetworkElementBase* movedNetworkElement);

    virtual void move(const qreal& dx, const qreal& dy) = 0;

protected:

    QList<MyNetworkElementBase*> _networkElements;
    MyNetworkElementBase* _movedNetworkElement;
};

class MyNodeMover : public MyNetworkElementMoverBase {
    Q_OBJECT

public:

    MyNodeMover(QList<MyNetworkElementBase*> networkElements, MyNetworkElementBase* movedNetworkElement);

    void move(const qreal& dx, const qreal& dy) override;

};

#endif
