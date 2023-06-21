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
    connect(addAction("Copy"), SIGNAL(triggered()), this, SIGNAL(askForCopySelectedNetworkElements()));
    connect(addAction("Paste"), SIGNAL(triggered()), this, SIGNAL(askForPasteCopiedNetworkElements()));
    connect(addAction("Remove"), SIGNAL(triggered()), this, SIGNAL(askForRemoveSelectedNetworkElements()));
}

void MyGraphicsSceneContextMenu::initializeActionsStatus() {
    MyContextMenuBase::initializeActionsStatus();
    if (!askForWhetherSelectedElementsAreCopyable())
        setActionEnabled("Copy", false);
    if (!askForWhetherAnyElementsAreCopied())
        setActionEnabled("Paste", false);
    if (!askForWhetherAnyElementsAreSelected())
        setActionEnabled("Remove", false);
}

// MyGraphicsItemContextMenuBase

MyGraphicsItemContextMenuBase::MyGraphicsItemContextMenuBase(QWidget *parent) : MyContextMenuBase(parent) {
    connect(addAction("Features"), SIGNAL(triggered()), this, SIGNAL(askForCreateFeatureMenu()));
}

void MyGraphicsItemContextMenuBase::initializeActionsStatus() {
    MyContextMenuBase::initializeActionsStatus();
    if (!askForWhetherElementStyleIsCopied())
        setActionEnabled("Paste Style", false);
}

// MyNodeGraphicsItemContextMenu

MyNodeGraphicsItemContextMenu::MyNodeGraphicsItemContextMenu(QWidget *parent) : MyGraphicsItemContextMenuBase(parent) {
    connect(addAction("Copy"), SIGNAL(triggered()), this, SIGNAL(askForCopyNetworkElement()));
    connect(addAction("Copy Style"), SIGNAL(triggered()), this, SIGNAL(askForCopyNetworkElementStyle()));
    connect(addAction("Paste"), SIGNAL(triggered()), this, SIGNAL(askForPasteNetworkElementStyle()));
    connect(addAction("Remove"), SIGNAL(triggered()), this, SIGNAL(askForRemoveNetworkElement()));
}

// MyEdgeGraphicsItemContextMenu

MyEdgeGraphicsItemContextMenu::MyEdgeGraphicsItemContextMenu(QWidget *parent) : MyGraphicsItemContextMenuBase(parent) {
    connect(addAction("Copy Style"), SIGNAL(triggered()), this, SIGNAL(askForCopyNetworkElementStyle()));
    connect(addAction("Paste Style"), SIGNAL(triggered()), this, SIGNAL(askForPasteNetworkElementStyle()));
    connect(addAction("Delete"), SIGNAL(triggered()), this, SIGNAL(askForDeleteNetworkElement()));
}

// MyArrowHeadGraphicsItemContextMenu

MyArrowHeadGraphicsItemContextMenu::MyArrowHeadGraphicsItemContextMenu(QWidget *parent) : MyGraphicsItemContextMenuBase(parent) {

}
