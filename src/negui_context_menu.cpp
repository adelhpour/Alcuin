#include "negui_context_menu.h"

// MyContextMenuBase

MyContextMenuBase::MyContextMenuBase(QWidget *parent) : QMenu(parent) {

}

void MyContextMenuBase::initializeActionsStatus() {
    for (QAction* action : actions())
        action->setEnabled(true);
}

void MyContextMenuBase::setActionEnabled(const QString& actionText, const bool& enabled) {
    for (QAction* action : actions()) {
        if (action->text() == actionText)
            action->setEnabled(enabled);
    }
}

// MyGraphicsSceneContextMenu

MyGraphicsSceneContextMenu::MyGraphicsSceneContextMenu(QWidget *parent) : MyContextMenuBase(parent) {
    connect(addAction("Copy"), SIGNAL(triggered()), this, SIGNAL(askForCopyNetworkElementStyle()));
    connect(addAction("Paste"), SIGNAL(triggered()), this, SIGNAL(askForPasteNetworkElement()));
}

void MyGraphicsSceneContextMenu::initializeActionsStatus() {
    MyContextMenuBase::initializeActionsStatus();
    if (!askForWhetherAnyElementsAreSelected())
        setActionEnabled("Copy", false);
    if (!askForWhetherCopiedElementStyleIsSet())
        setActionEnabled("Paste", false);
}

// MyGraphicsItemContextMenuBase

MyGraphicsItemContextMenuBase::MyGraphicsItemContextMenuBase(QWidget *parent) : MyContextMenuBase(parent) {
    connect(addAction("Features"), SIGNAL(triggered()), this, SIGNAL(askForCreateFeatureMenu()));
    connect(addAction("Copy"), SIGNAL(triggered()), this, SIGNAL(askForCopyNetworkElementStyle()));
    connect(addAction("Paste"), SIGNAL(triggered()), this, SIGNAL(askForPasteNetworkElementStyle()));
}

void MyGraphicsItemContextMenuBase::initializeActionsStatus() {
    MyContextMenuBase::initializeActionsStatus();
    if (!askForWhetherCopiedElementStyleIsSet())
        setActionEnabled("Paste", false);
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
