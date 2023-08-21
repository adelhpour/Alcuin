#ifndef __NEGUI_ELEMENT_ALIGNER_H
#define __NEGUI_ELEMENT_ALIGNER_H

#include "negui_network_element_base.h"

class MyElementAlignerBase : public QObject {
    Q_OBJECT

public:

    MyElementAlignerBase(QList<MyNetworkElementBase*> elements);

    virtual void align() = 0;

protected:

    QList<MyNetworkElementBase*> _elements;
    qreal _minX;
    qreal _minY;
    qreal _maxX;
    qreal _maxY;
};

class MyNodeAlignerBase : public MyElementAlignerBase {
    Q_OBJECT

public:

    MyNodeAlignerBase(QList<MyNetworkElementBase*> elements);

    void align() override;

    void extractExtents();

    virtual void adjustNodePositions() = 0;
};

class MyNodeTopAligner : public MyNodeAlignerBase {
    Q_OBJECT

public:

    MyNodeTopAligner(QList<MyNetworkElementBase*> elements);

    void adjustNodePositions() override;
};


class MyNodeMiddleAligner : public MyNodeAlignerBase {
    Q_OBJECT

public:

    MyNodeMiddleAligner(QList<MyNetworkElementBase*> elements);

    void adjustNodePositions() override;
};


class MyNodeBottomAligner : public MyNodeAlignerBase {
    Q_OBJECT

public:

    MyNodeBottomAligner(QList<MyNetworkElementBase*> elements);

    void adjustNodePositions() override;
};

class MyNodeLeftAligner : public MyNodeAlignerBase {
    Q_OBJECT

public:

    MyNodeLeftAligner(QList<MyNetworkElementBase*> elements);

    void adjustNodePositions() override;
};

class MyNodeCenterAligner : public MyNodeAlignerBase {
    Q_OBJECT

public:

    MyNodeCenterAligner(QList<MyNetworkElementBase*> elements);

    void adjustNodePositions() override;
};

class MyNodeRightAligner : public MyNodeAlignerBase {
    Q_OBJECT

public:

    MyNodeRightAligner(QList<MyNetworkElementBase*> elements);

    void adjustNodePositions() override;
};

class MyNodeDistributeAlignerBase : public MyNodeAlignerBase {
    Q_OBJECT

public:

    MyNodeDistributeAlignerBase(QList<MyNetworkElementBase*> elements);

    QList<MyNetworkElementBase*> getClassicNodes();
};

class MyNodeHorizontallyDistributeAligner : public MyNodeDistributeAlignerBase {
    Q_OBJECT

public:

    MyNodeHorizontallyDistributeAligner(QList<MyNetworkElementBase*> elements);

    void adjustNodePositions() override;
};

class MyNodeVerticallyDistributeAligner : public MyNodeDistributeAlignerBase {
    Q_OBJECT

public:

    MyNodeVerticallyDistributeAligner(QList<MyNetworkElementBase*> elements);

    void adjustNodePositions() override;
};


#endif
