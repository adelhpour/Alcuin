#ifndef __NEGUI_GRAPHICS_SCENE_H
#define __NEGUI_GRAPHICS_SCENE_H

#include "negui_scene_mode_element_base.h"

#include <QWidget>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>

class MyGraphicsScene :public QGraphicsScene, public MySceneModeElementBase {
    Q_OBJECT
    
public:
    
    MyGraphicsScene(QWidget* parent = nullptr);

    QMenu* createContextMenu();

    void connectContextMenu(QMenu* contextMenu);

    void updateViewFrame(const QRectF& viewRect);
    
signals:

    void askForCreateChangeStageCommand();
    void mousePositionIsChanged(const QPointF&);
    void mouseLeftButtonIsPressed(const QPointF&);
    void mousePressedLeftButtonIsMoved(const QPointF&);
    void mouseLeftButtonIsReleased();
    void escapeKeyIsPressed();
    void askForEnableNormalMode();
    void askForSelectAll();
    const bool askForWhetherSelectedElementsAreCopyable();
    const bool askForWhetherSelectedElementsAreCuttable();
    const bool askForWhetherAnyElementsAreAlignable();
    const bool askForWhetherAnyElementsAreCopied();
    const bool askForWhetherAnyElementsAreSelected();
    void askForCopySelectedNetworkElements();
    void askForCutSelectedNetworkElements();
    void askForPasteCopiedNetworkElements(const QPointF&);
    void askForDeleteSelectedNetworkElements();
    void askForAlignSelectedNetworkElements(const QString&);

public slots:
    
    void setSceneRect(qreal x, qreal y, qreal width, qreal height);
    void addGraphicsItem(QGraphicsItem* item);
    void removeGraphicsItem(QGraphicsItem* item);
    const QRectF networkExtents();
    const QString backgroundColor();
    void setBackgroundColor(const QString& backgroundColor);
    void clearScene();
    QList<QGraphicsItem *> itemsAtPosition(const QPointF& position);
    const bool isShiftModifierPressed();
    void displayContextMenu(const QPointF& position);
    const bool whetherMouseReleaseEventIsAccepted();
    
protected:
    
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    bool _isLeftButtonPressed;
    bool _isShiftModifierPressed;
    bool _whetherMouseReleaseEventIsAccepted;
    QPointF _cursorPosition;
    QGraphicsItem* _viewFrameGraphicsItem;
};

#endif
