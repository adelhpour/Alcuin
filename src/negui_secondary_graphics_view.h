#ifndef __NEGUI_SECONDARY_GRAPHICS_VIEW_H
#define __NEGUI_SECONDARY_GRAPHICS_VIEW_H

#include <QGraphicsView>

class MySecondaryGraphicsView : public QGraphicsView {
    Q_OBJECT

public:

    MySecondaryGraphicsView(QGraphicsScene* mainScene, QWidget* parent = nullptr);

    void adjustScale();
};

#endif
