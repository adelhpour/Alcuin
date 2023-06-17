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

    const bool askForWhetherSelectedElementsAreCopyable();

    const bool askForWhetherAnyElementsAreCopied();

    void askForCopySelectedNetworkElements();

    void askForPasteCopiedNetworkElements();
};

class MyGraphicsItemContextMenuBase : public MyContextMenuBase {
    Q_OBJECT

public:

    MyGraphicsItemContextMenuBase(QWidget *parent = nullptr);

    void initializeActionsStatus() override;

signals:

    const bool askForWhetherElementStyleIsCopied();

    void askForCreateFeatureMenu();

    void askForCopyNetworkElement();

    void askForCopyNetworkElementStyle();

    void askForPasteNetworkElementStyle();

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
