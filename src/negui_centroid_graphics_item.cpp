#include "negui_centroid_graphics_item.h"
#include "negui_centroid_style.h"
#include "negui_centroid_adjust_handled_graphics_item.h"

// MyCentroidGraphicsItem

MyCentroidGraphicsItem::MyCentroidGraphicsItem(qreal x, qreal y, QGraphicsItem *parent) : My2DShapeGraphicsItemBase(x, y), QGraphicsEllipseItem(parent) {

}

void MyCentroidGraphicsItem::updateStyle() {
    if (isSetStyle()) {
        // pen
        setPen(((MyCentroidStyleBase*)style())->pen());
        
        // brush
        setBrush(((MyCentroidStyleBase*)style())->brush());
        
        // position and dimensions
        setRect(_originalPosition.x() - ((MyCentroidStyleBase*)style())->radius(), _originalPosition.y() - ((MyCentroidStyleBase*)style())->radius(), 2 * ((MyCentroidStyleBase*)style())->radius(), 2 * ((MyCentroidStyleBase*)style())->radius());
    }
}

void MyCentroidGraphicsItem::setSelectedWithBorderColor(const bool& selected) {
    if (selected)
        setPen(((MyCentroidStyleBase*)style())->selectedPen());
    else
        setPen(((MyCentroidStyleBase*)style())->pen());
    QGraphicsItem::setSelected(selected);
}

void MyCentroidGraphicsItem::setSelectedWithFillColor(const bool& selected) {
    QGraphicsItem::setSelected(selected);
}

void MyCentroidGraphicsItem::updateExtents(const QRectF& extents) {
    
}

QRectF MyCentroidGraphicsItem::getExtents() {
    return QRectF(- ((MyCentroidStyleBase*)style())->radius() + _originalPosition.x() + _movedOriginalPosition.x(),
                  - ((MyCentroidStyleBase*)style())->radius() + _originalPosition.y() + _movedOriginalPosition.y(),
                  2 * ((MyCentroidStyleBase*)style())->radius(), 2 * ((MyCentroidStyleBase*)style())->radius());
}

void MyCentroidGraphicsItem::updateOriginalPosition(const QPointF originalPosition) {
    _originalPosition = originalPosition - _movedOriginalPosition;
}

void MyCentroidGraphicsItem::moveOriginalPosition(const qreal& dx, const qreal& dy) {
    _movedOriginalPosition += QPointF(dx, dy);
}

QGraphicsItem* MyCentroidGraphicsItem::getFocusedGraphicsItem() {
    MyCentroidAdjustHandledGraphicsItems* focusedGraphicsItem = new MyCentroidAdjustHandledGraphicsItems(getFocusedRect(), askForGetBezierAdjustLine(), zValue());
    connect(focusedGraphicsItem, SIGNAL(bezierAdjustLineIsUpdated(const QLineF&)), this, SIGNAL(bezierAdjustLineIsUpdated(const QLineF&)));
    return focusedGraphicsItem;
}

const QRectF MyCentroidGraphicsItem::getFocusedRect() {
    QRectF extentsRect = getExtents();
    qreal rectWidth = qMax(extentsRect.width(), 5.0);
    qreal rectHeight = qMax(extentsRect.height(), 5.0);
    return QRectF(extentsRect.x() - 0.5 * (rectWidth - extentsRect.width()), extentsRect.y() - 0.5 * (rectHeight - extentsRect.height()), rectWidth, rectHeight);
}

void MyCentroidGraphicsItem::setZValue(qreal z) {
    QGraphicsItem::setZValue(z);
};
