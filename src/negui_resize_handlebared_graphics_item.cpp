#include "negui_resize_handlebared_graphics_item.h"
#include <QPen>
#include <QCursor>

// MyResizeHandlebaredGraphicsItemBase

MyResizeHandlebaredGraphicsItemBase::MyResizeHandlebaredGraphicsItemBase(const QRectF &rect, qreal zValue, QGraphicsItem *parent) : QGraphicsItemGroup(parent) {
    setHandlesChildEvents(false);
    setZValue(zValue + 1);
    createResizeRectItem(rect);
}

void MyResizeHandlebaredGraphicsItemBase::createResizeRectItem(const QRectF &rect) {
    _resizeRect = new QGraphicsRectItem(rect);
    QPen pen;
    pen.setWidth(1.0);
    pen.setColor(QColor("#4169e1"));
    pen.setStyle(Qt::DashLine);
    ((QGraphicsRectItem*)_resizeRect)->setPen(pen);
    addToGroup(_resizeRect);
}

void MyResizeHandlebaredGraphicsItemBase::createResizeHandlbarItems(const QRectF &rect) {
    createMiddleLeftResizeHandlebar(rect);
    createTopLeftResizeHandlebar(rect);
    createTopMiddleResizeHandlebar(rect);
    createTopRightResizeHandlebar(rect);
    createMiddleRightResizeHandlebar(rect);
    createBottomRightResizeHandlebar(rect);
    createBottomMiddleResizeHandlebar(rect);
    createBottomLeftResizeHandlebar(rect);
}

void MyResizeHandlebaredGraphicsItemBase::createMiddleLeftResizeHandlebar(const QRectF &rect) {
    MyResizeHandlebarGraphicsItemBase* handlebarItem = new MyMiddleLeftResizeHandlebarGraphicsItem(rect);
    connect(handlebarItem, SIGNAL(offsetRectUpdated(const QRectF&)), this, SLOT(updateRectUsingOffsetRect(const QRectF&)));
    _resizeHandlebarItems.push_back(handlebarItem);
    addToGroup(handlebarItem);
}

void MyResizeHandlebaredGraphicsItemBase::createTopLeftResizeHandlebar(const QRectF &rect) {
    MyResizeHandlebarGraphicsItemBase* handlebarItem = new MyTopLeftResizeHandlebarGraphicsItem(rect);
    connect(handlebarItem, SIGNAL(offsetRectUpdated(const QRectF&)), this, SLOT(updateRectUsingOffsetRect(const QRectF&)));
    _resizeHandlebarItems.push_back(handlebarItem);
    addToGroup(handlebarItem);
}

void MyResizeHandlebaredGraphicsItemBase::createTopMiddleResizeHandlebar(const QRectF &rect) {
    MyResizeHandlebarGraphicsItemBase* handlebarItem = new MyTopMiddleResizeHandlebarGraphicsItem(rect);
    connect(handlebarItem, SIGNAL(offsetRectUpdated(const QRectF&)), this, SLOT(updateRectUsingOffsetRect(const QRectF&)));
    _resizeHandlebarItems.push_back(handlebarItem);
    addToGroup(handlebarItem);
}

void MyResizeHandlebaredGraphicsItemBase::createTopRightResizeHandlebar(const QRectF &rect) {
    MyResizeHandlebarGraphicsItemBase* handlebarItem = new MyTopRightResizeHandlebarGraphicsItem(rect);
    connect(handlebarItem, SIGNAL(offsetRectUpdated(const QRectF&)), this, SLOT(updateRectUsingOffsetRect(const QRectF&)));
    _resizeHandlebarItems.push_back(handlebarItem);
    addToGroup(handlebarItem);
}

void MyResizeHandlebaredGraphicsItemBase::createMiddleRightResizeHandlebar(const QRectF &rect) {
    MyResizeHandlebarGraphicsItemBase* handlebarItem = new MyMiddleRightResizeHandlebarGraphicsItem(rect);
    connect(handlebarItem, SIGNAL(offsetRectUpdated(const QRectF&)), this, SLOT(updateRectUsingOffsetRect(const QRectF&)));
    _resizeHandlebarItems.push_back(handlebarItem);
    addToGroup(handlebarItem);
}

void MyResizeHandlebaredGraphicsItemBase::createBottomRightResizeHandlebar(const QRectF &rect) {
    MyResizeHandlebarGraphicsItemBase* handlebarItem = new MyBottomRightResizeHandlebarGraphicsItem(rect);
    connect(handlebarItem, SIGNAL(offsetRectUpdated(const QRectF&)), this, SLOT(updateRectUsingOffsetRect(const QRectF&)));
    _resizeHandlebarItems.push_back(handlebarItem);
    addToGroup(handlebarItem);
}

void MyResizeHandlebaredGraphicsItemBase::createBottomMiddleResizeHandlebar(const QRectF &rect) {
    MyResizeHandlebarGraphicsItemBase* handlebarItem = new MyBottomMiddleResizeHandlebarGraphicsItem(rect);
    connect(handlebarItem, SIGNAL(offsetRectUpdated(const QRectF&)), this, SLOT(updateRectUsingOffsetRect(const QRectF&)));
    _resizeHandlebarItems.push_back(handlebarItem);
    addToGroup(handlebarItem);
}

void MyResizeHandlebaredGraphicsItemBase::createBottomLeftResizeHandlebar(const QRectF &rect) {
    MyResizeHandlebarGraphicsItemBase* handlebarItem = new MyBottomLeftResizeHandlebarGraphicsItem(rect);
    connect(handlebarItem, SIGNAL(offsetRectUpdated(const QRectF&)), this, SLOT(updateRectUsingOffsetRect(const QRectF&)));
    _resizeHandlebarItems.push_back(handlebarItem);
    addToGroup(handlebarItem);
}

void MyResizeHandlebaredGraphicsItemBase::updateRectUsingOffsetRect(const QRectF& offsetRect) {
    _resizeRect->setRect(_resizeRect->rect().x() + offsetRect.x(), _resizeRect->rect().y() + offsetRect.y(), qMax(_resizeRect->rect().width() + offsetRect.width(), 5.0), qMax(_resizeRect->rect().height() + offsetRect.height(), 5.0));
    updateHandlebarItemsPositions();
    emit rectIsUpdated(_resizeRect->rect());
}

void MyResizeHandlebaredGraphicsItemBase::updateHandlebarItemsPositions() {
    for (QGraphicsItem* handlebarItem : _resizeHandlebarItems)
        ((MyResizeHandlebarGraphicsItemBase*)handlebarItem)->updatePostion(_resizeRect->rect());
}

// MyResizeHandlebaredGraphicsItem

MyResizeHandlebaredGraphicsItem::MyResizeHandlebaredGraphicsItem(const QRectF &rect, qreal zValue, QGraphicsItem *parent) : MyResizeHandlebaredGraphicsItemBase(rect, zValue, parent) {
    createResizeHandlbarItems(rect);
}

// MyRoundedRectangleResizeHandlebaredGraphicsItem

MyRoundedRectangleResizeHandlebaredGraphicsItem::MyRoundedRectangleResizeHandlebaredGraphicsItem(const QRectF &rect, qreal radiusX, qreal radiusY, qreal zValue, QGraphicsItem *parent) : MyResizeHandlebaredGraphicsItemBase(rect, zValue, parent) {
    _radiusX = radiusX;
    _radiusY = radiusY;
    createResizeHandlbarItems(rect, radiusX, radiusY);
}

void MyRoundedRectangleResizeHandlebaredGraphicsItem::createResizeHandlbarItems(const QRectF &rect, qreal radiusX, qreal radiusY) {
    MyResizeHandlebaredGraphicsItemBase::createResizeHandlbarItems(rect);
    createCurvatureRadiusXControlHandlebar(rect, radiusX);
    createCurvatureRadiusYControlHandlebar(rect, radiusY);
}

void MyRoundedRectangleResizeHandlebaredGraphicsItem::createCurvatureRadiusXControlHandlebar(const QRectF &rect, qreal radius) {
    MyCurvatureRadiusControlHandlebarGraphicsItemBase* handlebarItem = new MyCurvatureRadiusXControlHandlebarGraphicsItem(rect, radius);
    connect(handlebarItem, SIGNAL(handlebarPositionUpdated(const qreal&)), this, SLOT(updateRectUsingRadiusXHandlebarPosition(const qreal&)));
    _curvatureRadiusXHandlebarItem = handlebarItem;
    addToGroup(handlebarItem);
}

void MyRoundedRectangleResizeHandlebaredGraphicsItem::createCurvatureRadiusYControlHandlebar(const QRectF &rect, qreal radius) {
    MyCurvatureRadiusControlHandlebarGraphicsItemBase* handlebarItem = new MyCurvatureRadiusYControlHandlebarGraphicsItem(rect, radius);
    connect(handlebarItem, SIGNAL(handlebarPositionUpdated(const qreal&)), this, SLOT(updateRectUsingRadiusYHandlebarPosition(const qreal&)));
    _curvatureRadiusYHandlebarItem = handlebarItem;
    addToGroup(handlebarItem);
}

void MyRoundedRectangleResizeHandlebaredGraphicsItem::updateRectUsingOffsetRect(const QRectF& offsetRect) {
    _radiusX = ((_resizeRect->rect().width() + offsetRect.width()) / _resizeRect->rect().width()) * _radiusX;
    _radiusY = ((_resizeRect->rect().height() + offsetRect.height()) / _resizeRect->rect().height()) * _radiusY;
    MyResizeHandlebaredGraphicsItemBase::updateRectUsingOffsetRect(offsetRect);
}

void MyRoundedRectangleResizeHandlebaredGraphicsItem::updateRectUsingRadiusXHandlebarPosition(const qreal& radiusX) {
    _radiusX = radiusX;
    updateHandlebarItemsPositions();
    emit curvatureRadiiAreUpdated(_radiusX, _radiusY);
}

void MyRoundedRectangleResizeHandlebaredGraphicsItem::updateRectUsingRadiusYHandlebarPosition(const qreal& radiusY) {
    _radiusY = radiusY;
    updateHandlebarItemsPositions();
    emit curvatureRadiiAreUpdated(_radiusX, _radiusY);
}

void MyRoundedRectangleResizeHandlebaredGraphicsItem::updateHandlebarItemsPositions() {
    MyResizeHandlebaredGraphicsItemBase::updateHandlebarItemsPositions();
    ((MyCurvatureRadiusControlHandlebarGraphicsItemBase*)_curvatureRadiusXHandlebarItem)->updatePostion(_resizeRect->rect(), _radiusX);
    ((MyCurvatureRadiusControlHandlebarGraphicsItemBase*)_curvatureRadiusYHandlebarItem)->updatePostion(_resizeRect->rect(), _radiusY);
}

// MyHandlebarGraphicsItemBase

MyHandlebarGraphicsItemBase::MyHandlebarGraphicsItemBase(QGraphicsItem *parent) : QGraphicsEllipseItem(parent) {
    setCursor(Qt::PointingHandCursor);
    _handlebarRadius = 2.0;
}

void MyHandlebarGraphicsItemBase::updatePostion(const QPointF& center) {
    setRect(QRectF(center.x() - _handlebarRadius, center.y() - _handlebarRadius, 2 * _handlebarRadius, 2 * _handlebarRadius));
}

void MyHandlebarGraphicsItemBase::updatePostion(const QRectF &rect) {
    _rect = rect;
}

void MyHandlebarGraphicsItemBase::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    _pressedPosition = event->scenePos();
}

// MyResizeHandlebarGraphicsItemBase

MyResizeHandlebarGraphicsItemBase::MyResizeHandlebarGraphicsItemBase(QGraphicsItem *parent) : MyHandlebarGraphicsItemBase(parent) {
    QPen pen;
    pen.setWidth(1.0);
    pen.setColor(QColor("#4169e1"));
    setPen(pen);
    
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor("#4169e1"));
    setBrush(brush);
}

const bool MyResizeHandlebarGraphicsItemBase::isUpwardMovePossbile(const QPointF& eventScenePosition) {
    return eventScenePosition.y() > _rect.y();
}

const bool MyResizeHandlebarGraphicsItemBase::isDownwardMovePossbile(const QPointF& eventScenePosition) {
    return eventScenePosition.y() - _pressedPosition.y() < _rect.height();
}

const bool MyResizeHandlebarGraphicsItemBase::isRightwardMovePossbile(const QPointF& eventScenePosition) {
    return eventScenePosition.x() - _pressedPosition.x() < _rect.width();
}

const bool MyResizeHandlebarGraphicsItemBase::isLeftwardMovePossbile(const QPointF& eventScenePosition) {
    return eventScenePosition.x() > _rect.x();
}

// MyMiddleLeftResizeHandlebarGraphicsItem

MyMiddleLeftResizeHandlebarGraphicsItem::MyMiddleLeftResizeHandlebarGraphicsItem(const QRectF &rect, QGraphicsItem *parent) : MyResizeHandlebarGraphicsItemBase(parent) {
    updatePostion(rect);
}

void MyMiddleLeftResizeHandlebarGraphicsItem::updatePostion(const QRectF &rect) {
    MyHandlebarGraphicsItemBase::updatePostion(rect);
    MyHandlebarGraphicsItemBase::updatePostion(QPointF(rect.x(), rect.y() + 0.5 * rect.height()));
}

void MyMiddleLeftResizeHandlebarGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (isRightwardMovePossbile(event->scenePos())) {
        emit offsetRectUpdated(QRectF(event->scenePos().x() - _pressedPosition.x(), 0.0, _pressedPosition.x() - event->scenePos().x(), 0.0));
        _pressedPosition = event->scenePos();
    }
    QGraphicsItem::mouseMoveEvent(event);
}

// MyTopLeftResizeHandlebarGraphicsItem

MyTopLeftResizeHandlebarGraphicsItem::MyTopLeftResizeHandlebarGraphicsItem(const QRectF &rect, QGraphicsItem *parent) : MyResizeHandlebarGraphicsItemBase(parent) {
    updatePostion(rect);
}

void MyTopLeftResizeHandlebarGraphicsItem::updatePostion(const QRectF &rect) {
    MyHandlebarGraphicsItemBase::updatePostion(rect);
    MyHandlebarGraphicsItemBase::updatePostion(rect.topLeft());
}

void MyTopLeftResizeHandlebarGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
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

// MyTopMiddleResizeHandlebarGraphicsItem

MyTopMiddleResizeHandlebarGraphicsItem::MyTopMiddleResizeHandlebarGraphicsItem(const QRectF &rect, QGraphicsItem *parent) : MyResizeHandlebarGraphicsItemBase(parent) {
    updatePostion(rect);
}

void MyTopMiddleResizeHandlebarGraphicsItem::updatePostion(const QRectF &rect) {
    MyHandlebarGraphicsItemBase::updatePostion(rect);
    MyHandlebarGraphicsItemBase::updatePostion(QPointF(rect.x() + 0.5 * rect.width(), rect.y()));
}

void MyTopMiddleResizeHandlebarGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (isDownwardMovePossbile(event->scenePos())) {
        emit offsetRectUpdated(QRectF(0.0, event->scenePos().y() - _pressedPosition.y(), 0.0, _pressedPosition.y() - event->scenePos().y()));
        _pressedPosition = event->scenePos();
    }
    QGraphicsItem::mouseMoveEvent(event);
}

// MyTopRightResizeHandlebarGraphicsItem

MyTopRightResizeHandlebarGraphicsItem::MyTopRightResizeHandlebarGraphicsItem(const QRectF &rect, QGraphicsItem *parent) : MyResizeHandlebarGraphicsItemBase(parent) {
    updatePostion(rect);
}

void MyTopRightResizeHandlebarGraphicsItem::updatePostion(const QRectF &rect) {
    MyHandlebarGraphicsItemBase::updatePostion(rect);
    MyHandlebarGraphicsItemBase::updatePostion(rect.topRight());
}

void MyTopRightResizeHandlebarGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
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

// MyMiddleRightResizeHandlebarGraphicsItem

MyMiddleRightResizeHandlebarGraphicsItem::MyMiddleRightResizeHandlebarGraphicsItem(const QRectF &rect, QGraphicsItem *parent) : MyResizeHandlebarGraphicsItemBase(parent) {
    MyResizeHandlebarGraphicsItemBase::updatePostion(QPointF(rect.x() + rect.width(), rect.y() + 0.5 * rect.height()));
}

void MyMiddleRightResizeHandlebarGraphicsItem::updatePostion(const QRectF &rect) {
    MyHandlebarGraphicsItemBase::updatePostion(rect);
    MyHandlebarGraphicsItemBase::updatePostion(QPointF(rect.x() + rect.width(), rect.y() + 0.5 * rect.height()));
}

void MyMiddleRightResizeHandlebarGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (isLeftwardMovePossbile(event->scenePos())) {
        emit offsetRectUpdated(QRectF(0.0, 0.0, event->scenePos().x() - _pressedPosition.x(), 0.0));
        _pressedPosition = event->scenePos();
    }
    QGraphicsItem::mouseMoveEvent(event);
}

// MyBottomRightResizeHandlebarGraphicsItem

MyBottomRightResizeHandlebarGraphicsItem::MyBottomRightResizeHandlebarGraphicsItem(const QRectF &rect, QGraphicsItem *parent) : MyResizeHandlebarGraphicsItemBase(parent) {
    updatePostion(rect);
}

void MyBottomRightResizeHandlebarGraphicsItem::updatePostion(const QRectF &rect) {
    MyHandlebarGraphicsItemBase::updatePostion(rect);
    MyHandlebarGraphicsItemBase::updatePostion(rect.bottomRight());
}

void MyBottomRightResizeHandlebarGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
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

// MyBottomMiddleResizeHandlebarGraphicsItem

MyBottomMiddleResizeHandlebarGraphicsItem::MyBottomMiddleResizeHandlebarGraphicsItem(const QRectF &rect, QGraphicsItem *parent) : MyResizeHandlebarGraphicsItemBase(parent) {
    updatePostion(rect);
}

void MyBottomMiddleResizeHandlebarGraphicsItem::updatePostion(const QRectF &rect) {
    MyHandlebarGraphicsItemBase::updatePostion(rect);
    MyHandlebarGraphicsItemBase::updatePostion(QPointF(rect.x() + 0.5 * rect.width(), rect.y() + rect.height()));
}

void MyBottomMiddleResizeHandlebarGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (isUpwardMovePossbile(event->scenePos())) {
        emit offsetRectUpdated(QRectF(0.0, 0.0, 0.0, event->scenePos().y() - _pressedPosition.y()));
        _pressedPosition = event->scenePos();
    }
    QGraphicsItem::mouseMoveEvent(event);
}

// MyBottomLeftResizeHandlebarGraphicsItem

MyBottomLeftResizeHandlebarGraphicsItem::MyBottomLeftResizeHandlebarGraphicsItem(const QRectF &rect, QGraphicsItem *parent) : MyResizeHandlebarGraphicsItemBase(parent) {
    updatePostion(rect);
}

void MyBottomLeftResizeHandlebarGraphicsItem::updatePostion(const QRectF &rect) {
    MyHandlebarGraphicsItemBase::updatePostion(rect);
    MyHandlebarGraphicsItemBase::updatePostion(rect.bottomLeft());
}

void MyBottomLeftResizeHandlebarGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
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

// MyCurvatureRadiusControlHandlebarGraphicsItemBase

MyCurvatureRadiusControlHandlebarGraphicsItemBase::MyCurvatureRadiusControlHandlebarGraphicsItemBase(QGraphicsItem *parent) : MyHandlebarGraphicsItemBase(parent) {
    QPen pen;
    pen.setWidth(1.0);
    pen.setColor(QColor("#DAA520"));
    setPen(pen);
    
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor("#DAA520"));
    setBrush(brush);
}

void MyCurvatureRadiusControlHandlebarGraphicsItemBase::updatePostion(const QRectF &rect, qreal radius) {
    MyHandlebarGraphicsItemBase::updatePostion(rect);
    _radius = radius;
}

// MyCurvatureRadiusXControlHandlebarGraphicsItem

MyCurvatureRadiusXControlHandlebarGraphicsItem::MyCurvatureRadiusXControlHandlebarGraphicsItem(const QRectF &rect, qreal radius, QGraphicsItem *parent) : MyCurvatureRadiusControlHandlebarGraphicsItemBase(parent) {
    updatePostion(rect, radius);
}

void MyCurvatureRadiusXControlHandlebarGraphicsItem::updatePostion(const QRectF &rect, qreal radius) {
    MyCurvatureRadiusControlHandlebarGraphicsItemBase::updatePostion(rect, radius);
    MyHandlebarGraphicsItemBase::updatePostion(rect.topLeft() + QPointF(qMin(radius, 0.5 * rect.width()), 0.0));
}

void MyCurvatureRadiusXControlHandlebarGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    emit handlebarPositionUpdated(qMax(qMin((event->scenePos().x() - _rect.x()), 0.5 * _rect.width()), 0.0));
    QGraphicsItem::mouseMoveEvent(event);
}

// MyCurvatureRadiusYControlHandlebarGraphicsItem

MyCurvatureRadiusYControlHandlebarGraphicsItem::MyCurvatureRadiusYControlHandlebarGraphicsItem(const QRectF &rect, qreal radius, QGraphicsItem *parent) : MyCurvatureRadiusControlHandlebarGraphicsItemBase(parent) {
    updatePostion(rect, radius);
}

void MyCurvatureRadiusYControlHandlebarGraphicsItem::updatePostion(const QRectF &rect, qreal radius) {
    MyCurvatureRadiusControlHandlebarGraphicsItemBase::updatePostion(rect, radius);
    MyHandlebarGraphicsItemBase::updatePostion(rect.topLeft() + QPointF(0.0, qMin(radius, 0.5 * rect.height())));
}

void MyCurvatureRadiusYControlHandlebarGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    emit handlebarPositionUpdated(qMax(qMin((event->scenePos().y() - _rect.y()), 0.5 * _rect.height()), 0.0));
    QGraphicsItem::mouseMoveEvent(event);
}

