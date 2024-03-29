#ifndef __NEGUI_NETWORK_ELEMENT_ALIGNER_H
#define __NEGUI_NETWORK_ELEMENT_ALIGNER_H

#include "negui_network_element_base.h"
#include "negui_customized_common_widgets.h"

class MyNetworkElementAlignerBase : public QObject {
    Q_OBJECT

public:

    MyNetworkElementAlignerBase(QList<MyNetworkElementBase*> networkElements);

    virtual void align() = 0;

protected:

    QList<MyNetworkElementBase*> _networkElements;
    qreal _minX;
    qreal _minY;
    qreal _maxX;
    qreal _maxY;
};

class MyNodeAlignerBase : public MyNetworkElementAlignerBase {
    Q_OBJECT

public:

    MyNodeAlignerBase(QList<MyNetworkElementBase*> networkElements);

    void align() override;

    void extractExtents();

    QList<MyNetworkElementBase*> getClassicNodes();

    QList<MyNetworkElementBase*> getCentroidNodes();

    virtual void adjustNodePositions() = 0;

    void adjustCentroidNodePositions();
};

class MyNodeTopAligner : public MyNodeAlignerBase {
    Q_OBJECT

public:

    MyNodeTopAligner(QList<MyNetworkElementBase*> networkElements);

    void adjustNodePositions() override;
};


class MyNodeMiddleAligner : public MyNodeAlignerBase {
    Q_OBJECT

public:

    MyNodeMiddleAligner(QList<MyNetworkElementBase*> networkElements);

    void adjustNodePositions() override;
};


class MyNodeBottomAligner : public MyNodeAlignerBase {
    Q_OBJECT

public:

    MyNodeBottomAligner(QList<MyNetworkElementBase*> networkElements);

    void adjustNodePositions() override;
};

class MyNodeLeftAligner : public MyNodeAlignerBase {
    Q_OBJECT

public:

    MyNodeLeftAligner(QList<MyNetworkElementBase*> networkElements);

    void adjustNodePositions() override;
};

class MyNodeCenterAligner : public MyNodeAlignerBase {
    Q_OBJECT

public:

    MyNodeCenterAligner(QList<MyNetworkElementBase*> networkElements);

    void adjustNodePositions() override;
};

class MyNodeRightAligner : public MyNodeAlignerBase {
    Q_OBJECT

public:

    MyNodeRightAligner(QList<MyNetworkElementBase*> networkElements);

    void adjustNodePositions() override;
};

class MyNodeDistributeAlignerBase : public MyNodeAlignerBase {
    Q_OBJECT

public:

    MyNodeDistributeAlignerBase(QList<MyNetworkElementBase*> networkElements);
};

class MyNodeHorizontallyDistributeAligner : public MyNodeDistributeAlignerBase {
    Q_OBJECT

public:

    MyNodeHorizontallyDistributeAligner(QList<MyNetworkElementBase*> networkElements);

    void adjustNodePositions() override;
};

class MyNodeVerticallyDistributeAligner : public MyNodeDistributeAlignerBase {
    Q_OBJECT

public:

    MyNodeVerticallyDistributeAligner(QList<MyNetworkElementBase*> networkElements);

    void adjustNodePositions() override;
};

class MyNodeGridDistributeAligner : public MyNodeDistributeAlignerBase {
    Q_OBJECT

public:

    MyNodeGridDistributeAligner(QList<MyNetworkElementBase*> networkElements);

    void adjustNodePositions() override;

    qint32 getNumberOfGridColumns(QList<MyNetworkElementBase*> classicNodes);

    void adjustClassicNodesPositionsInGrid(QList<MyNetworkElementBase*> classicNodes, const qint32& numberOfGridColumns);
};

class MyGetNumberOfGridColumnsDialog : public MyDialog {
    Q_OBJECT

public:

    MyGetNumberOfGridColumnsDialog(MyParameterBase* parameter, QWidget *parent = nullptr);
};


#endif
