#include "negui_line_graphics_item.h"
#include "negui_line_style.h"
#include <QtMath>

// MyLineGraphicsItem

MyLineGraphicsItem::MyLineGraphicsItem(const QLineF& line, QGraphicsItem *parent) : QGraphicsPathItem(parent) {
    setLine(line);
}

void MyLineGraphicsItem::updateStyle() {
    if (isSetStyle()) {
        // pen
        setPen(((MyLineStyle*)style())->pen());
    }
}

void MyLineGraphicsItem::setSelectedWithStroke(const bool& selected) {
    if (selected)
        setPen(style()->selectedPen());
    else
        setPen(((MyLineStyle*)style())->pen());
    QGraphicsItem::setSelected(selected);
}

void MyLineGraphicsItem::setSelectedWithFill(const bool& selected) {
    QGraphicsItem::setSelected(selected);
}

QRectF MyLineGraphicsItem::getExtents() {
    return boundingRect();
}

void MyLineGraphicsItem::setZValue(qreal z) {
    QGraphicsItem::setZValue(z);
};

void MyLineGraphicsItem::setLine(const QLineF& line) {
    _line = line;
    _endSlope = qAtan2(_line.p2().y() - _line.p1().y(), _line.p2().x() - _line.p1().x());
    QPainterPath _path;
    _path.moveTo(_line.p1());
    _path.lineTo(_line.p2());
    setPath(_path);
}
