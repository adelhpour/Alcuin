#include "negui_graphics_scene.h"
#include "negui_context_menu.h"

// MyGraphicsScene

MyGraphicsScene::MyGraphicsScene(QWidget* parent) : QGraphicsScene(parent) {
    setSceneRect(30.0, 20.0, 840.0, 560.0);
    _isLeftButtonPressed = false;
    _isShiftModifierPressed = false;
}

void MyGraphicsScene::setSceneRect(qreal x, qreal y, qreal width, qreal height) {
    QGraphicsScene::setSceneRect(x, y, width, height);
}

void MyGraphicsScene::addGraphicsItem(QGraphicsItem* item) {
    addItem(item);
}

void MyGraphicsScene::removeGraphicsItem(QGraphicsItem* item) {
    removeItem(item);
}

void MyGraphicsScene::clearScene() {
    clear();
}

QList<QGraphicsItem *> MyGraphicsScene::itemsAtPosition(const QPointF& position) {
    return QGraphicsScene::items(position);
}

const bool MyGraphicsScene::isShiftModifierPressed() {
    return _isShiftModifierPressed;
}

void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);
    if (!event->isAccepted()) {
        if (event->button() == Qt::LeftButton) {
            qint32 numberOfItems = items().size();
            emit mouseLeftButtonIsPressed(event->scenePos());
            
            // if a node is added
            if (items().size() != numberOfItems)
                event->accept();
            _isLeftButtonPressed = true;
        }
    }
}

void MyGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (_isLeftButtonPressed)
        emit mousePressedLeftButtonIsMoved(event->scenePos());
    QGraphicsScene::mouseMoveEvent(event);
}

void MyGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    _isLeftButtonPressed = false;
    emit mouseLeftButtonIsReleased();
    QGraphicsScene::mouseReleaseEvent(event);
}

void MyGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    emit mouseLeftButtonIsDoubleClicked();
    QGraphicsScene::mouseDoubleClickEvent(event);
}

void MyGraphicsScene::keyPressEvent(QKeyEvent *event) {
    QGraphicsScene::keyPressEvent(event);
    if (!event->isAccepted()) {
        if (event->key() == Qt::Key_Escape) {
            emit escapeKeyIsPressed();
            event->accept();
        }
        else if (event->modifiers() == Qt::ShiftModifier) {
            _isShiftModifierPressed = true;
            event->accept();
        }
        else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_A)
            emit askForSelectAll();
    }
}

void MyGraphicsScene::keyReleaseEvent(QKeyEvent *event) {
    QGraphicsScene::keyReleaseEvent(event);
    if (!event->isAccepted()) {
        if (event->key() == Qt::Key_Shift) {
            _isShiftModifierPressed = false;
            event->accept();
        }
    }
}

void MyGraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    QGraphicsScene::contextMenuEvent(event);
    if (!event->isAccepted()) {
        MyGraphicsSceneContextMenu contextMenu;
        contextMenu.exec(QPoint(event->scenePos().x(), event->scenePos().y()));
        event->accept();
    }
}
