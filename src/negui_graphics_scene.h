#ifndef __NEGUI_GRAPHICS_SCENE_H
#define __NEGUI_GRAPHICS_SCENE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>

class MyGraphicsScene : public QGraphicsScene {
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
    void escapeKeyIsPressed();
    void askForSelectAll();
    const bool askForWhetherAnyCopyableElementsAreSelected();
    const bool askForWhetherAnyElementsAreCopied();
    void askForCopyNetworkElements();
    void askForPasteNetworkElements(const QPointF& position);

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
    QPointF _mousePressedPosition;
};

#endif
