#ifndef __NEGUI_GRAPHICS_SCENE_H
#define __NEGUI_GRAPHICS_SCENE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

class MyGraphicsScene : public QGraphicsScene {
    Q_OBJECT
    
public:
    
    MyGraphicsScene(QWidget* parent = nullptr);
    
signals:
    
    void mouseLeftButtonIsPressed(const QPointF& position);
    void mousePressedLeftButtonIsMoved(const QPointF& position);
    void mouseLeftButtonIsReleased();
    void mouseLeftButtonIsDoubleClicked();
    void escapeKeyIsPressed();
    void askForSelectAll();

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
};

#endif
