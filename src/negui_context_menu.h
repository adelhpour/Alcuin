#ifndef __NEGUI_CONTEXT_MENU_H
#define __NEGUI_CONTEXT_MENU_H

#include <QMenu>

class MyContextMenuBase : public QMenu {
    Q_OBJECT

public:

    MyContextMenuBase(QWidget *parent = nullptr);
};

class MyGraphicsSceneContextMenu : public MyContextMenuBase {
    Q_OBJECT

public:

    MyGraphicsSceneContextMenu(QWidget *parent = nullptr);
};

class MyGraphicsItemContextMenuBase : public MyContextMenuBase {
    Q_OBJECT

public:

    MyGraphicsItemContextMenuBase(QWidget *parent = nullptr);
};

class MyNodeGraphicsItemContextMenu : public MyGraphicsItemContextMenuBase {
    Q_OBJECT

public:

    MyNodeGraphicsItemContextMenu(QWidget *parent = nullptr);
};

class MyEdgeGraphicsItemContextMenu : public MyGraphicsItemContextMenuBase {
    Q_OBJECT

public:

    MyEdgeGraphicsItemContextMenu(QWidget *parent = nullptr);
};

class MyArrowHeadGraphicsItemContextMenu : public MyGraphicsItemContextMenuBase {
    Q_OBJECT

public:

    MyArrowHeadGraphicsItemContextMenu(QWidget *parent = nullptr);
};

#endif
