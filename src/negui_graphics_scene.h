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
    
signals:
    
    void mouseLeftButtonIsPressed(const QPointF& position);
    void mousePressedLeftButtonIsMoved(const QPointF& position);
    void mouseLeftButtonIsReleased();
    void mouseLeftButtonIsDoubleClicked();
    void mouseRightButtonIsPressed();
    void escapeKeyIsPressed();
    void askForSelectAll();
    const bool askForWhetherSelectedElementsAreCopyable();
    const bool askForWhetherAnyElementsAreCopied();
    const bool askForWhetherAnyElementsAreSelected();
    void askForCopySelectedNetworkElements();
    void askForPasteCopiedNetworkElements(const QPointF& position);
    void askForRemoveSelectedNetworkElements();

public slots:
    
    void setSceneRect(qreal x, qreal y, qreal width, qreal height);
    void addGraphicsItem(QGraphicsItem* item);
    void removeGraphicsItem(QGraphicsItem* item);
    void clearScene();
    QList<QGraphicsItem *> itemsAtPosition(const QPointF& position);
    const bool isShiftModifierPressed();
    void displayContextMenu(const QPointF& position);
    
protected:
    
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    bool _isLeftButtonPressed;
    bool _isShiftModifierPressed;
    QPointF _cursorPosition;
};

#endif
