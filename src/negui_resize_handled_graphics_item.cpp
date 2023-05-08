#include "negui_resize_handled_graphics_item.h"
#include <QPen>
#include <QCursor>

// MyResizeHandledGraphicsItemBase

MyResizeHandledGraphicsItemBase::MyResizeHandledGraphicsItemBase(const QRectF &rect, qreal zValue, QGraphicsItem *parent) : QGraphicsItemGroup(parent) {
    setHandlesChildEvents(false);
    setZValue(zValue + 1);
    createResizeRectItem(rect);
}

void MyResizeHandledGraphicsItemBase::createResizeRectItem(const QRectF &rect) {
    _resizeRect = new QGraphicsRectItem(rect);
    QPen pen;
    pen.setWidth(1.0);
    pen.setColor(QColor("#4169e1"));
    pen.setStyle(Qt::DashLine);
    ((QGraphicsRectItem*)_resizeRect)->setPen(pen);
    addToGroup(_resizeRect);
}

void MyResizeHandledGraphicsItemBase::createResizeHandledItems(const QRectF &rect) {
    createMiddleLeftResizeHandle(rect);
    createTopLeftResizeHandle(rect);
    createTopMiddleResizeHandle(rect);
    createTopRightResizeHandle(rect);
    createMiddleRightResizeHandle(rect);
    createBottomRightResizeHandle(rect);
    createBottomMiddleResizeHandle(rect);
    createBottomLeftResizeHandle(rect);
}

void MyResizeHandledGraphicsItemBase::createMiddleLeftResizeHandle(const QRectF &rect) {
    MyResizeHandleGraphicsItemBase* handleItem = new MyMiddleLeftResizeHandleGraphicsItem(rect);
    connect(handleItem, SIGNAL(offsetRectUpdated(const QRectF&)), this, SLOT(updateRectUsingOffsetRect(const QRectF&)));
    _resizeHandleItems.push_back(handleItem);
    addToGroup(handleItem);
}

void MyResizeHandledGraphicsItemBase::createTopLeftResizeHandle(const QRectF &rect) {
    MyResizeHandleGraphicsItemBase* handleItem = new MyTopLeftResizeHandleGraphicsItem(rect);
    connect(handleItem, SIGNAL(offsetRectUpdated(const QRectF&)), this, SLOT(updateRectUsingOffsetRect(const QRectF&)));
    _resizeHandleItems.push_back(handleItem);
    addToGroup(handleItem);
}

void MyResizeHandledGraphicsItemBase::createTopMiddleResizeHandle(const QRectF &rect) {
    MyResizeHandleGraphicsItemBase* handleItem = new MyTopMiddleResizeHandleGraphicsItem(rect);
    connect(handleItem, SIGNAL(offsetRectUpdated(const QRectF&)), this, SLOT(updateRectUsingOffsetRect(const QRectF&)));
    _resizeHandleItems.push_back(handleItem);
    addToGroup(handleItem);
}

void MyResizeHandledGraphicsItemBase::createTopRightResizeHandle(const QRectF &rect) {
    MyResizeHandleGraphicsItemBase* handleItem = new MyTopRightResizeHandleGraphicsItem(rect);
    connect(handleItem, SIGNAL(offsetRectUpdated(const QRectF&)), this, SLOT(updateRectUsingOffsetRect(const QRectF&)));
    _resizeHandleItems.push_back(handleItem);
    addToGroup(handleItem);
}

void MyResizeHandledGraphicsItemBase::createMiddleRightResizeHandle(const QRectF &rect) {
    MyResizeHandleGraphicsItemBase* handleItem = new MyMiddleRightResizeHandleGraphicsItem(rect);
    connect(handleItem, SIGNAL(offsetRectUpdated(const QRectF&)), this, SLOT(updateRectUsingOffsetRect(const QRectF&)));
    _resizeHandleItems.push_back(handleItem);
    addToGroup(handleItem);
}

void MyResizeHandledGraphicsItemBase::createBottomRightResizeHandle(const QRectF &rect) {
    MyResizeHandleGraphicsItemBase* handleItem = new MyBottomRightResizeHandleGraphicsItem(rect);
    connect(handleItem, SIGNAL(offsetRectUpdated(const QRectF&)), this, SLOT(updateRectUsingOffsetRect(const QRectF&)));
    _resizeHandleItems.push_back(handleItem);
    addToGroup(handleItem);
}

void MyResizeHandledGraphicsItemBase::createBottomMiddleResizeHandle(const QRectF &rect) {
    MyResizeHandleGraphicsItemBase* handleItem = new MyBottomMiddleResizeHandleGraphicsItem(rect);
    connect(handleItem, SIGNAL(offsetRectUpdated(const QRectF&)), this, SLOT(updateRectUsingOffsetRect(const QRectF&)));
    _resizeHandleItems.push_back(handleItem);
    addToGroup(handleItem);
}

void MyResizeHandledGraphicsItemBase::createBottomLeftResizeHandle(const QRectF &rect) {
    MyResizeHandleGraphicsItemBase* handleItem = new MyBottomLeftResizeHandleGraphicsItem(rect);
    connect(handleItem, SIGNAL(offsetRectUpdated(const QRectF&)), this, SLOT(updateRectUsingOffsetRect(const QRectF&)));
    _resizeHandleItems.push_back(handleItem);
    addToGroup(handleItem);
}

void MyResizeHandledGraphicsItemBase::updateRectUsingOffsetRect(const QRectF& offsetRect) {
    _resizeRect->setRect(_resizeRect->rect().x() + offsetRect.x(), _resizeRect->rect().y() + offsetRect.y(), qMax(_resizeRect->rect().width() + offsetRect.width(), 5.0), qMax(_resizeRect->rect().height() + offsetRect.height(), 5.0));
    updateHandleItemsPositions();
    emit rectIsUpdated(_resizeRect->rect());
}

void MyResizeHandledGraphicsItemBase::updateHandleItemsPositions() {
    for (QGraphicsItem* handleItem : _resizeHandleItems)
        ((MyResizeHandleGraphicsItemBase*)handleItem)->updatePosition(_resizeRect->rect());
}

// MyResizeHandledGraphicsItem

MyResizeHandledGraphicsItem::MyResizeHandledGraphicsItem(const QRectF &rect, qreal zValue, QGraphicsItem *parent) : MyResizeHandledGraphicsItemBase(rect, zValue, parent) {
    createResizeHandledItems(rect);
}

// MyRoundedRectangleResizeHandledGraphicsItem

MyRoundedRectangleResizeHandledGraphicsItem::MyRoundedRectangleResizeHandledGraphicsItem(const QRectF &rect, qreal radiusX, qreal radiusY, qreal zValue, QGraphicsItem *parent) : MyResizeHandledGraphicsItemBase(rect, zValue, parent) {
    _radiusX = radiusX;
    _radiusY = radiusY;
    createResizeHandledItems(rect, radiusX, radiusY);
}

void MyRoundedRectangleResizeHandledGraphicsItem::createResizeHandledItems(const QRectF &rect, qreal radiusX, qreal radiusY) {
    MyResizeHandledGraphicsItemBase::createResizeHandledItems(rect);
    createCurvatureRadiusXControlHandle(rect, radiusX);
    createCurvatureRadiusYControlHandle(rect, radiusY);
}

void MyRoundedRectangleResizeHandledGraphicsItem::createCurvatureRadiusXControlHandle(const QRectF &rect, qreal radius) {
    MyCurvatureRadiusControlHandleGraphicsItemBase* handleItem = new MyCurvatureRadiusXControlHandleGraphicsItem(rect, radius);
    connect(handleItem, SIGNAL(handlePositionUpdated(const qreal&)), this, SLOT(updateRectUsingRadiusXHandlePosition(const qreal&)));
    _curvatureRadiusXHandleItem = handleItem;
    addToGroup(handleItem);
}

void MyRoundedRectangleResizeHandledGraphicsItem::createCurvatureRadiusYControlHandle(const QRectF &rect, qreal radius) {
    MyCurvatureRadiusControlHandleGraphicsItemBase* handleItem = new MyCurvatureRadiusYControlHandleGraphicsItem(rect, radius);
    connect(handleItem, SIGNAL(handlePositionUpdated(const qreal&)), this, SLOT(updateRectUsingRadiusYHandlePosition(const qreal&)));
    _curvatureRadiusYHandleItem = handleItem;
    addToGroup(handleItem);
}

void MyRoundedRectangleResizeHandledGraphicsItem::updateRectUsingOffsetRect(const QRectF& offsetRect) {
    _radiusX = ((_resizeRect->rect().width() + offsetRect.width()) / _resizeRect->rect().width()) * _radiusX;
    _radiusY = ((_resizeRect->rect().height() + offsetRect.height()) / _resizeRect->rect().height()) * _radiusY;
    MyResizeHandledGraphicsItemBase::updateRectUsingOffsetRect(offsetRect);
}

void MyRoundedRectangleResizeHandledGraphicsItem::updateRectUsingRadiusXHandlePosition(const qreal& radiusX) {
    _radiusX = radiusX;
    updateHandleItemsPositions();
    emit curvatureRadiiAreUpdated(_radiusX, _radiusY);
}

void MyRoundedRectangleResizeHandledGraphicsItem::updateRectUsingRadiusYHandlePosition(const qreal& radiusY) {
    _radiusY = radiusY;
    updateHandleItemsPositions();
    emit curvatureRadiiAreUpdated(_radiusX, _radiusY);
}

void MyRoundedRectangleResizeHandledGraphicsItem::updateHandleItemsPositions() {
    MyResizeHandledGraphicsItemBase::updateHandleItemsPositions();
    ((MyCurvatureRadiusControlHandleGraphicsItemBase*)_curvatureRadiusXHandleItem)->updatePosition(_resizeRect->rect(), _radiusX);
    ((MyCurvatureRadiusControlHandleGraphicsItemBase*)_curvatureRadiusYHandleItem)->updatePosition(_resizeRect->rect(), _radiusY);
}

// MyHandleGraphicsItemBase

MyHandleGraphicsItemBase::MyHandleGraphicsItemBase(QGraphicsItem *parent) : QGraphicsEllipseItem(parent) {
    setCursor(Qt::PointingHandCursor);
    _handleRadius = 2.0;
}

void MyHandleGraphicsItemBase::updatePosition(const QPointF& center) {
    setRect(QRectF(center.x() - _handleRadius, center.y() - _handleRadius, 2 * _handleRadius, 2 * _handleRadius));
}

void MyHandleGraphicsItemBase::updatePosition(const QRectF &rect) {
    _rect = rect;
}

void MyHandleGraphicsItemBase::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    _pressedPosition = event->scenePos();
}

// MyResizeHandleGraphicsItemBase

MyResizeHandleGraphicsItemBase::MyResizeHandleGraphicsItemBase(QGraphicsItem *parent) : MyHandleGraphicsItemBase(parent) {
    QPen pen;
    pen.setWidth(1.0);
    pen.setColor(QColor("#4169e1"));
    setPen(pen);
    
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor("#4169e1"));
    setBrush(brush);
}

const bool MyResizeHandleGraphicsItemBase::isUpwardMovePossbile(const QPointF& eventScenePosition) {
    return eventScenePosition.y() > _rect.y();
}

const bool MyResizeHandleGraphicsItemBase::isDownwardMovePossbile(const QPointF& eventScenePosition) {
    return eventScenePosition.y() - _pressedPosition.y() < _rect.height();
}

const bool MyResizeHandleGraphicsItemBase::isRightwardMovePossbile(const QPointF& eventScenePosition) {
    return eventScenePosition.x() - _pressedPosition.x() < _rect.width();
}

const bool MyResizeHandleGraphicsItemBase::isLeftwardMovePossbile(const QPointF& eventScenePosition) {
    return eventScenePosition.x() > _rect.x();
}

// MyMiddleLeftResizeHandleGraphicsItem

MyMiddleLeftResizeHandleGraphicsItem::MyMiddleLeftResizeHandleGraphicsItem(const QRectF &rect, QGraphicsItem *parent) : MyResizeHandleGraphicsItemBase(parent) {
    updatePosition(rect);
}

void MyMiddleLeftResizeHandleGraphicsItem::updatePosition(const QRectF &rect) {
    MyHandleGraphicsItemBase::updatePosition(rect);
    MyHandleGraphicsItemBase::updatePosition(QPointF(rect.x(), rect.y() + 0.5 * rect.height()));
}

void MyMiddleLeftResizeHandleGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (isRightwardMovePossbile(event->scenePos())) {
        emit offsetRectUpdated(QRectF(event->scenePos().x() - _pressedPosition.x(), 0.0, _pressedPosition.x() - event->scenePos().x(), 0.0));
        _pressedPosition = event->scenePos();
    }
    QGraphicsItem::mouseMoveEvent(event);
}

// MyTopLeftResizeHandleGraphicsItem

MyTopLeftResizeHandleGraphicsItem::MyTopLeftResizeHandleGraphicsItem(const QRectF &rect, QGraphicsItem *parent) : MyResizeHandleGraphicsItemBase(parent) {
    updatePosition(rect);
}

void MyTopLeftResizeHandleGraphicsItem::updatePosition(const QRectF &rect) {
    MyHandleGraphicsItemBase::updatePosition(rect);
    MyHandleGraphicsItemBase::updatePosition(rect.topLeft());
}

void MyTopLeftResizeHandleGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (isRightwardMovePossbile(event->scenePos()) && isDownwardMovePossbile(event->scenePos())) {
        emit offsetRectUpdated(QRectF(event->scenePos().x() - _pressedPosition.x(), event->scenePos().y() - _pressedPosition.y(), _pressedPosition.x() - event->scenePos().x(), _pressedPosition.y() - event->scenePos().y()));
        _pressedPosition = event->scenePos();
    }
    else if (isRightwardMovePossbile(event->scenePos())) {
        emit offsetRectUpdated(QRectF(event->scenePos().x() - _pressedPosition.x(), 0.0, _pressedPosition.x() - event->scenePos().x(), 0.0));
        _pressedPosition = event->scenePos();
    }
    else if (isDownwardMovePossbile(event->scenePos())) {
        emit offsetRectUpdated(QRectF(0.0, event->scenePos().y() - _pressedPosition.y(), 0.0, _pressedPosition.y() - event->scenePos().y()));
        _pressedPosition = event->scenePos();
    }
    QGraphicsItem::mouseMoveEvent(event);
}

// MyTopMiddleResizeHandleGraphicsItem

MyTopMiddleResizeHandleGraphicsItem::MyTopMiddleResizeHandleGraphicsItem(const QRectF &rect, QGraphicsItem *parent) : MyResizeHandleGraphicsItemBase(parent) {
    updatePosition(rect);
}

void MyTopMiddleResizeHandleGraphicsItem::updatePosition(const QRectF &rect) {
    MyHandleGraphicsItemBase::updatePosition(rect);
    MyHandleGraphicsItemBase::updatePosition(QPointF(rect.x() + 0.5 * rect.width(), rect.y()));
}

void MyTopMiddleResizeHandleGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (isDownwardMovePossbile(event->scenePos())) {
        emit offsetRectUpdated(QRectF(0.0, event->scenePos().y() - _pressedPosition.y(), 0.0, _pressedPosition.y() - event->scenePos().y()));
        _pressedPosition = event->scenePos();
    }
    QGraphicsItem::mouseMoveEvent(event);
}

// MyTopRightResizeHandleGraphicsItem

MyTopRightResizeHandleGraphicsItem::MyTopRightResizeHandleGraphicsItem(const QRectF &rect, QGraphicsItem *parent) : MyResizeHandleGraphicsItemBase(parent) {
    updatePosition(rect);
}

void MyTopRightResizeHandleGraphicsItem::updatePosition(const QRectF &rect) {
    MyHandleGraphicsItemBase::updatePosition(rect);
    MyHandleGraphicsItemBase::updatePosition(rect.topRight());
}

void MyTopRightResizeHandleGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (isLeftwardMovePossbile(event->scenePos()) && isDownwardMovePossbile(event->scenePos())) {
        emit offsetRectUpdated(QRectF(0.0, event->scenePos().y() - _pressedPosition.y(), event->scenePos().x() - _pressedPosition.x(), _pressedPosition.y() - event->scenePos().y()));
        _pressedPosition = event->scenePos();
    }
    else if (isLeftwardMovePossbile(event->scenePos())) {
        emit offsetRectUpdated(QRectF(0.0, 0.0, event->scenePos().x() - _pressedPosition.x(), 0.0));
        _pressedPosition = event->scenePos();
    }
    else if (isDownwardMovePossbile(event->scenePos())) {
        emit offsetRectUpdated(QRectF(0.0, event->scenePos().y() - _pressedPosition.y(), 0.0, _pressedPosition.y() - event->scenePos().y()));
        _pressedPosition = event->scenePos();
    }
    QGraphicsItem::mouseMoveEvent(event);
}

// MyMiddleRightResizeHandleGraphicsItem

MyMiddleRightResizeHandleGraphicsItem::MyMiddleRightResizeHandleGraphicsItem(const QRectF &rect, QGraphicsItem *parent) : MyResizeHandleGraphicsItemBase(parent) {
    MyResizeHandleGraphicsItemBase::updatePosition(QPointF(rect.x() + rect.width(), rect.y() + 0.5 * rect.height()));
}

void MyMiddleRightResizeHandleGraphicsItem::updatePosition(const QRectF &rect) {
    MyHandleGraphicsItemBase::updatePosition(rect);
    MyHandleGraphicsItemBase::updatePosition(QPointF(rect.x() + rect.width(), rect.y() + 0.5 * rect.height()));
}

void MyMiddleRightResizeHandleGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (isLeftwardMovePossbile(event->scenePos())) {
        emit offsetRectUpdated(QRectF(0.0, 0.0, event->scenePos().x() - _pressedPosition.x(), 0.0));
        _pressedPosition = event->scenePos();
    }
    QGraphicsItem::mouseMoveEvent(event);
}

// MyBottomRightResizeHandleGraphicsItem

MyBottomRightResizeHandleGraphicsItem::MyBottomRightResizeHandleGraphicsItem(const QRectF &rect, QGraphicsItem *parent) : MyResizeHandleGraphicsItemBase(parent) {
    updatePosition(rect);
}

void MyBottomRightResizeHandleGraphicsItem::updatePosition(const QRectF &rect) {
    MyHandleGraphicsItemBase::updatePosition(rect);
    MyHandleGraphicsItemBase::updatePosition(rect.bottomRight());
}

void MyBottomRightResizeHandleGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (isLeftwardMovePossbile(event->scenePos()) && isUpwardMovePossbile(event->scenePos())) {
        emit offsetRectUpdated(QRectF(0.0, 0.0, event->scenePos().x() - _pressedPosition.x(), event->scenePos().y() - _pressedPosition.y()));
        _pressedPosition = event->scenePos();
    }
    else if (isLeftwardMovePossbile(event->scenePos())) {
        emit offsetRectUpdated(QRectF(0.0, 0.0, event->scenePos().x() - _pressedPosition.x(), 0.0));
        _pressedPosition = event->scenePos();
    }
    else if (isUpwardMovePossbile(event->scenePos())) {
        emit offsetRectUpdated(QRectF(0.0, 0.0, 0.0, event->scenePos().y() - _pressedPosition.y()));
        _pressedPosition = event->scenePos();
    }
    QGraphicsItem::mouseMoveEvent(event);
}

// MyBottomMiddleResizeHandleGraphicsItem

MyBottomMiddleResizeHandleGraphicsItem::MyBottomMiddleResizeHandleGraphicsItem(const QRectF &rect, QGraphicsItem *parent) : MyResizeHandleGraphicsItemBase(parent) {
    updatePosition(rect);
}

void MyBottomMiddleResizeHandleGraphicsItem::updatePosition(const QRectF &rect) {
    MyHandleGraphicsItemBase::updatePosition(rect);
    MyHandleGraphicsItemBase::updatePosition(QPointF(rect.x() + 0.5 * rect.width(), rect.y() + rect.height()));
}

void MyBottomMiddleResizeHandleGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (isUpwardMovePossbile(event->scenePos())) {
        emit offsetRectUpdated(QRectF(0.0, 0.0, 0.0, event->scenePos().y() - _pressedPosition.y()));
        _pressedPosition = event->scenePos();
    }
    QGraphicsItem::mouseMoveEvent(event);
}

// MyBottomLeftResizeHandleGraphicsItem

MyBottomLeftResizeHandleGraphicsItem::MyBottomLeftResizeHandleGraphicsItem(const QRectF &rect, QGraphicsItem *parent) : MyResizeHandleGraphicsItemBase(parent) {
    updatePosition(rect);
}

void MyBottomLeftResizeHandleGraphicsItem::updatePosition(const QRectF &rect) {
    MyHandleGraphicsItemBase::updatePosition(rect);
    MyHandleGraphicsItemBase::updatePosition(rect.bottomLeft());
}

void MyBottomLeftResizeHandleGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (isUpwardMovePossbile(event->scenePos()) && isRightwardMovePossbile(event->scenePos())) {
        emit offsetRectUpdated(QRectF(event->scenePos().x() - _pressedPosition.x(), 0.0, _pressedPosition.x() - event->scenePos().x(), event->scenePos().y() - _pressedPosition.y()));
        _pressedPosition = event->scenePos();
    }
    else if (isUpwardMovePossbile(event->scenePos())) {
        emit offsetRectUpdated(QRectF(0.0, 0.0, 0.0, event->scenePos().y() - _pressedPosition.y()));
        _pressedPosition = event->scenePos();
    }
    else if (isRightwardMovePossbile(event->scenePos())) {
        emit offsetRectUpdated(QRectF(event->scenePos().x() - _pressedPosition.x(), 0.0, _pressedPosition.x() - event->scenePos().x(), 0.0));
        _pressedPosition = event->scenePos();
    }
    QGraphicsItem::mouseMoveEvent(event);
}

// MyCurvatureRadiusControlHandleGraphicsItemBase

MyCurvatureRadiusControlHandleGraphicsItemBase::MyCurvatureRadiusControlHandleGraphicsItemBase(QGraphicsItem *parent) : MyHandleGraphicsItemBase(parent) {
    QPen pen;
    pen.setWidth(1.0);
    pen.setColor(QColor("#DAA520"));
    setPen(pen);
    
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor("#DAA520"));
    setBrush(brush);
}

void MyCurvatureRadiusControlHandleGraphicsItemBase::updatePosition(const QRectF &rect, qreal radius) {
    MyHandleGraphicsItemBase::updatePosition(rect);
    _radius = radius;
}

// MyCurvatureRadiusXControlHandleGraphicsItem

MyCurvatureRadiusXControlHandleGraphicsItem::MyCurvatureRadiusXControlHandleGraphicsItem(const QRectF &rect, qreal radius, QGraphicsItem *parent) : MyCurvatureRadiusControlHandleGraphicsItemBase(parent) {
    updatePosition(rect, radius);
}

void MyCurvatureRadiusXControlHandleGraphicsItem::updatePosition(const QRectF &rect, qreal radius) {
    MyCurvatureRadiusControlHandleGraphicsItemBase::updatePosition(rect, radius);
    MyHandleGraphicsItemBase::updatePosition(rect.topLeft() + QPointF(qMin(radius, 0.5 * rect.width()), 0.0));
}

void MyCurvatureRadiusXControlHandleGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    emit handlePositionUpdated(qMax(qMin((event->scenePos().x() - _rect.x()), 0.5 * _rect.width()), 0.0));
    QGraphicsItem::mouseMoveEvent(event);
}

// MyCurvatureRadiusYControlHandleGraphicsItem

MyCurvatureRadiusYControlHandleGraphicsItem::MyCurvatureRadiusYControlHandleGraphicsItem(const QRectF &rect, qreal radius, QGraphicsItem *parent) : MyCurvatureRadiusControlHandleGraphicsItemBase(parent) {
    updatePosition(rect, radius);
}

void MyCurvatureRadiusYControlHandleGraphicsItem::updatePosition(const QRectF &rect, qreal radius) {
    MyCurvatureRadiusControlHandleGraphicsItemBase::updatePosition(rect, radius);
    MyHandleGraphicsItemBase::updatePosition(rect.topLeft() + QPointF(0.0, qMin(radius, 0.5 * rect.height())));
}

void MyCurvatureRadiusYControlHandleGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    emit handlePositionUpdated(qMax(qMin((event->scenePos().y() - _rect.y()), 0.5 * _rect.height()), 0.0));
    QGraphicsItem::mouseMoveEvent(event);
}

