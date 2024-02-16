#ifndef __NEGUI_CONTEXT_MENU_H
#define __NEGUI_CONTEXT_MENU_H

#include <QMenu>

class MyContextMenuBase : public QMenu {
    Q_OBJECT

public:

    MyContextMenuBase(QWidget *parent = nullptr);

    void setActionEnabled(const QString& actionText, const bool& enabled);

    virtual void initializeActionsStatus();
};

class MyGraphicsSceneContextMenu : public MyContextMenuBase {
    Q_OBJECT

public:

    MyGraphicsSceneContextMenu(QWidget *parent = nullptr);

    void initializeActionsStatus() override;

signals:

    bool askForWhetherSelectedElementsAreCopyable();

    bool askForWhetherSelectedElementsAreCuttable();

    bool askForWhetherAnyElementsAreAlignable();

    bool askForWhetherAnyElementsAreCopied();

    bool askForWhetherAnyElementsAreSelected();

    void askForCopySelectedNetworkElements();

    void askForCutSelectedNetworkElements();

    void askForPasteCopiedNetworkElements();

    void askForDeleteSelectedNetworkElements();

    void askForAlignSelectedNetworkElements(const QString&);

    void askForSetNetworkBackgroundColor();
};

class MyGraphicsItemContextMenuBase : public MyContextMenuBase {
    Q_OBJECT

public:

    MyGraphicsItemContextMenuBase(QWidget *parent = nullptr);

    void initializeActionsStatus() override;

signals:

    const bool askForWhetherElementStyleIsCopied();

    void askForDisplayFeatureMenu();

    void askForCopyNetworkElement();

    void askForCutNetworkElement();

    void askForCopyNetworkElementStyle();

    void askForPasteNetworkElementStyle();

    void askForDeleteNetworkElement();
};

class MyNodeGraphicsItemContextMenuBase : public MyGraphicsItemContextMenuBase {
    Q_OBJECT

public:

    MyNodeGraphicsItemContextMenuBase(QWidget *parent = nullptr);
};

class MyCentroidNodeGraphicsItemContextMenu : public MyNodeGraphicsItemContextMenuBase {
    Q_OBJECT

public:

    MyCentroidNodeGraphicsItemContextMenu(QWidget *parent = nullptr);

    void initializeActionsStatus() override;

signals:

    const bool askForWhetherNodePositionIsConnectedToNeighborNodes();

    void askForConnectNodePositionToNeighborNodes(const bool&);
};

class MyEdgeGraphicsItemContextMenu : public MyGraphicsItemContextMenuBase {
    Q_OBJECT

public:

    MyEdgeGraphicsItemContextMenu(QWidget *parent = nullptr);
};

#endif
