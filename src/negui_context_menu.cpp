#include "negui_context_menu.h"

// MyContextMenuBase

MyContextMenuBase::MyContextMenuBase(QWidget *parent) : QMenu(parent) {

}

// MyGraphicsSceneContextMenu

MyGraphicsSceneContextMenu::MyGraphicsSceneContextMenu(QWidget *parent) : MyContextMenuBase(parent) {
    addAction("sceneAct");
}

// MyGraphicsItemContextMenuBase

MyGraphicsItemContextMenuBase::MyGraphicsItemContextMenuBase(QWidget *parent) : MyContextMenuBase(parent) {

}

// MyNodeGraphicsItemContextMenu

MyNodeGraphicsItemContextMenu::MyNodeGraphicsItemContextMenu(QWidget *parent) : MyGraphicsItemContextMenuBase(parent) {
    addAction("nodeAct");
}

// MyEdgeGraphicsItemContextMenu

MyEdgeGraphicsItemContextMenu::MyEdgeGraphicsItemContextMenu(QWidget *parent) : MyGraphicsItemContextMenuBase(parent) {
    addAction("edgeAct");
}

// MyArrowHeadGraphicsItemContextMenu

MyArrowHeadGraphicsItemContextMenu::MyArrowHeadGraphicsItemContextMenu(QWidget *parent) : MyGraphicsItemContextMenuBase(parent) {
    addAction("arrowheadAct");
}
