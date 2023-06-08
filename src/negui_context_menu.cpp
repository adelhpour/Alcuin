#include "negui_context_menu.h"

// MyContextMenuBase

MyContextMenuBase::MyContextMenuBase(QWidget *parent) : QMenu(parent) {

}

// MyGraphicsSceneContextMenu

MyGraphicsSceneContextMenu::MyGraphicsSceneContextMenu(QWidget *parent) : MyContextMenuBase(parent) {
    addAction("cutAct");
}

// MyGraphicsItemContextMenu

MyGraphicsItemContextMenu::MyGraphicsItemContextMenu(QWidget *parent) : MyContextMenuBase(parent) {
    addAction("copyAct");
    addAction("pasteAct");
}
