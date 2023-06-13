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
    connect(addAction("Features"), SIGNAL(triggered()), this, SIGNAL(askForCreateFeatureMenu()));
    connect(addAction("Copy"), SIGNAL(triggered()), this, SIGNAL(askForCopyNetworkElementStyle()));
}

// MyNodeGraphicsItemContextMenu

MyNodeGraphicsItemContextMenu::MyNodeGraphicsItemContextMenu(QWidget *parent) : MyGraphicsItemContextMenuBase(parent) {

}

// MyEdgeGraphicsItemContextMenu

MyEdgeGraphicsItemContextMenu::MyEdgeGraphicsItemContextMenu(QWidget *parent) : MyGraphicsItemContextMenuBase(parent) {

}

// MyArrowHeadGraphicsItemContextMenu

MyArrowHeadGraphicsItemContextMenu::MyArrowHeadGraphicsItemContextMenu(QWidget *parent) : MyGraphicsItemContextMenuBase(parent) {

}
