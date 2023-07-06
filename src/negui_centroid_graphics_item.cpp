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

void MyCentroidGraphicsItem::setSelectedWithStroke(const bool& selected) {
    if (selected)
        setPen(((MyCentroidStyleBase*)style())->selectedPen());
    else
        setPen(((MyCentroidStyleBase*)style())->pen());
    QGraphicsItem::setSelected(selected);
}

void MyCentroidGraphicsItem::setSelectedWithFill(const bool& selected) {
    QGraphicsItem::setSelected(selected);
}

void MyCentroidGraphicsItem::updateExtents(const QRectF& extents) {
    
}

QRectF MyCentroidGraphicsItem::getExtents() {
    return QRectF(- ((MyCentroidStyleBase*)style())->radius() + (movedDistance().x() + _originalPosition.x()), - ((MyCentroidStyleBase*)style())->radius() + (movedDistance().y() + _originalPosition.y()), 2 * ((MyCentroidStyleBase*)style())->radius(), 2 * ((MyCentroidStyleBase*)style())->radius());
}

void MyCentroidGraphicsItem::adjustOriginalPosition(const QPointF& originalPositionMovedDistance) {

}

QGraphicsItem* MyCentroidGraphicsItem::getFocusedGraphicsItem() {
    MyCentroidAdjustHandledGraphicsItems* focusedGraphicsItem = new MyCentroidAdjustHandledGraphicsItems(getFocusedRect(), QLineF(), zValue());
    connect(focusedGraphicsItem, SIGNAL(lineIsUpdated(const QLineF&)), this, SLOT(updateAdjustLine(const QLineF&)));
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
