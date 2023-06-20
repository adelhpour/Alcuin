#ifndef __NEGUI_COPIED_NETWORK_ELEMENTS_PASTER_H
#define __NEGUI_COPIED_NETWORK_ELEMENTS_PASTER_H

#include "negui_network_element_base.h"
#include <QObject>

class MyCopiedNetworkElementsPaster : public QObject {
    Q_OBJECT

public:

    MyCopiedNetworkElementsPaster(QList<MyNetworkElementBase*> copiedNetworkElements, const QPointF& _copyCenter);

    void paste();

    void pasteNodes();

    MyNetworkElementBase* createPastedNode(MyNetworkElementBase* node, const QPointF& movedCenter);

    MyNetworkElementBase* createPastedEdge(MyNetworkElementBase* edge);

    void pasteEdges();

    const QPointF calculateElementsCenter();

    MyNetworkElementBase* findPastedNode(const QString& name);

signals:

    void askForAddEdge(MyNetworkElementBase*);

    void askForAddNode(MyNetworkElementBase*);

    QString askForNodeUniqueName(MyNetworkElementStyleBase*);

    QString askForEdgeUniqueName(MyNetworkElementStyleBase*);

protected:

    QList<MyNetworkElementBase*> _copiedNetworkElements;

    QList<MyNetworkElementBase*> _pastedNodes;

    QPointF _copyCenter;

    QHash <QString, QString> _nodeNamesHash;
};

MyNetworkElementStyleBase* getCopyNodeStyle(const QString& name, MyNetworkElementStyleBase* nodeStyle);
MyNetworkElementStyleBase* getCopyEdgeStyle(const QString& name, MyNetworkElementStyleBase* edgeStyle);

#endif
