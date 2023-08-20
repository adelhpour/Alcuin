#include "negui_context_menu.h"
#include "negui_customized_context_menu_widgets.h"

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
    connect(addAction("Cut"), SIGNAL(triggered()), this, SIGNAL(askForCutSelectedNetworkElements()));
    connect(addAction("Paste"), SIGNAL(triggered()), this, SIGNAL(askForPasteCopiedNetworkElements()));
    connect(addAction("Delete"), SIGNAL(triggered()), this, SIGNAL(askForDeleteSelectedNetworkElements()));
    connect(addAction("Align"), SIGNAL(triggered()), this, SIGNAL(askForAlignSelectedNetworkElements()));
}

void MyGraphicsSceneContextMenu::initializeActionsStatus() {
    MyContextMenuBase::initializeActionsStatus();
    if (!askForWhetherSelectedElementsAreCopyable())
        setActionEnabled("Copy", false);
    if (!askForWhetherSelectedElementsAreCuttable())
        setActionEnabled("Cut", false);
    if (!askForWhetherAnyElementsAreCopied())
        setActionEnabled("Paste", false);
    if (!askForWhetherAnyElementsAreSelected())
        setActionEnabled("Delete", false);
    if (!askForWhetherAnyElementsAreAlignable())
        setActionEnabled("Align", false);
}

// MyGraphicsItemContextMenuBase

MyGraphicsItemContextMenuBase::MyGraphicsItemContextMenuBase(QWidget *parent) : MyContextMenuBase(parent) {
    connect(addAction("Features"), SIGNAL(triggered()), this, SIGNAL(askForCreateFeatureMenu()));
}

void MyGraphicsItemContextMenuBase::initializeActionsStatus() {
    MyContextMenuBase::initializeActionsStatus();
    if (!askForWhetherElementStyleIsCopied())
        setActionEnabled("Paste", false);
}

// MyNodeGraphicsItemContextMenuBase

MyNodeGraphicsItemContextMenuBase::MyNodeGraphicsItemContextMenuBase(QWidget *parent) : MyGraphicsItemContextMenuBase(parent) {
    connect(addAction("Copy"), SIGNAL(triggered()), this, SIGNAL(askForCopyNetworkElement()));
    connect(addAction("Cut"), SIGNAL(triggered()), this, SIGNAL(askForCutNetworkElement()));
    connect(addAction("Copy Style"), SIGNAL(triggered()), this, SIGNAL(askForCopyNetworkElementStyle()));
    connect(addAction("Paste"), SIGNAL(triggered()), this, SIGNAL(askForPasteNetworkElementStyle()));
    connect(addAction("Delete"), SIGNAL(triggered()), this, SIGNAL(askForDeleteNetworkElement()));
}

// MyCentroidNodeGraphicsItemContextMenu

MyCentroidNodeGraphicsItemContextMenu::MyCentroidNodeGraphicsItemContextMenu(QWidget *parent) : MyNodeGraphicsItemContextMenuBase(parent) {
    QAction* lockPositionAction = new MyCheckableAction("Independent Position");
    addAction(lockPositionAction);
    connect(lockPositionAction, &QAction::triggered, this, [this, lockPositionAction] () { emit askForConnectNodePositionToNeighborNodes(!lockPositionAction->isChecked()); });
}

void MyCentroidNodeGraphicsItemContextMenu::initializeActionsStatus() {
    MyContextMenuBase::initializeActionsStatus();
    for (QAction* action : actions()) {
        if (action->text() == "Independent Position")
            action->setChecked(!askForWhetherNodePositionIsConnectedToNeighborNodes());
    }
}

// MyEdgeGraphicsItemContextMenu

MyEdgeGraphicsItemContextMenu::MyEdgeGraphicsItemContextMenu(QWidget *parent) : MyGraphicsItemContextMenuBase(parent) {
    connect(addAction("Copy Style"), SIGNAL(triggered()), this, SIGNAL(askForCopyNetworkElementStyle()));
    connect(addAction("Paste"), SIGNAL(triggered()), this, SIGNAL(askForPasteNetworkElementStyle()));
    connect(addAction("Delete"), SIGNAL(triggered()), this, SIGNAL(askForDeleteNetworkElement()));
}

// MyArrowHeadGraphicsItemContextMenu

MyArrowHeadGraphicsItemContextMenu::MyArrowHeadGraphicsItemContextMenu(QWidget *parent) : MyGraphicsItemContextMenuBase(parent) {

}
