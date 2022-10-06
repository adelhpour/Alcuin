#include "negui_graphics_scene.h"

// MyGraphicsScene

MyGraphicsScene::MyGraphicsScene(QWidget* parent) : QGraphicsScene(parent) {
    setSceneRect(30.0, 20.0, 840.0, 560.0);
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

void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);
    if (!event->isAccepted()) {
        if (event->button() == Qt::LeftButton) {
            qint32 numberOfItems = items().size();
            emit mouseLeftButtonIsPressed(event->scenePos());
            
            // if a node is added
            if (items().size() != numberOfItems)
                event->accept();
        }
        else if (event->button() == Qt::RightButton)
            emit mouseRightButtonIsPressed();
    }
}

void MyGraphicsScene::keyPressEvent(QKeyEvent *event) {
    QGraphicsScene::keyPressEvent(event);
    if (!event->isAccepted()) {
        if (event->key() == Qt::Key_Escape) {
            emit escapeKeyIsPressed();
            event->accept();
        }
    }
}
