#ifndef __NEGUI_SELECTION_AREA_BUILDER_H
#define __NEGUI_SELECTION_AREA_BUILDER_H

#include <QObject>
#include <QGraphicsItem>

class MySelectionAreaBuilder : public QObject {
    Q_OBJECT

public:
    MySelectionAreaBuilder();

    void build(const QPointF& position);

    const bool isSelectionAreaBuilt() { return _isSelectionAreaBuilt; }

signals:
    void askForAddGraphicsItem(QGraphicsItem*);
    void askForRemoveGraphicsItem(QGraphicsItem*);

protected:
    bool _isSelectionAreaBuilt;
};

#endif
