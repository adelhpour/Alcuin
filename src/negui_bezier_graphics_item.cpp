#include "negui_bezier_graphics_item.h"
#include "negui_bezier_style.h"
#include <QtMath>

// MyBezierGraphicsItem

MyBezierGraphicsItem::MyBezierGraphicsItem(const QLineF& line, QGraphicsItem *parent) : QGraphicsPathItem(parent) {
    setLine(line);
}

void MyBezierGraphicsItem::updateStyle() {
    if (isSetStyle()) {
        // pen
        setPen(((MyBezierStyle*)style())->pen());
        
        // line
        setLine(_line);
    }
}

void MyBezierGraphicsItem::setSelectedWithStroke(const bool& selected) {
    if (selected)
        setPen(style()->selectedPen());
    else
        setPen(((MyBezierStyle*)style())->pen());
    QGraphicsItem::setSelected(selected);
}

void MyBezierGraphicsItem::setSelectedWithFill(const bool& selected) {
    QGraphicsItem::setSelected(selected);
}

QRectF MyBezierGraphicsItem::getExtents() {
    return boundingRect();
}

void MyBezierGraphicsItem::setZValue(qreal z) {
    QGraphicsItem::setZValue(z);
};

void MyBezierGraphicsItem::setLine(const QLineF& line) {
    _line = line;
    _endSlope = qAtan2(_line.p2().y() - _line.p1().y(), _line.p2().x() - _line.p1().x());
    QPainterPath _path;
    QPointF controlPoint1 = _line.p1();
    QPointF controlPoint2 = _line.p2();
    if (isSetStyle()) {
        controlPoint1 += QPointF(0.01 * ((MyBezierStyle*)style())->relativeP1().x() * (_line.p2() - _line.p1()).x(), 0.01 * ((MyBezierStyle*)style())->relativeP1().y() * (_line.p2() - _line.p1()).y());
        controlPoint2 += QPointF(0.01 * ((MyBezierStyle*)style())->relativeP2().x() * (_line.p2() - _line.p1()).x(), 0.01 * ((MyBezierStyle*)style())->relativeP2().y() * (_line.p2() - _line.p1()).y());
        _endSlope = calcualteBezierEndSlope(_line.p1(), controlPoint1, controlPoint2, _line.p2());
    }
    _path.moveTo(_line.p1());
    _path.cubicTo(controlPoint1, controlPoint2,_line.p2());
    setPath(_path);
}

const qreal MyBezierGraphicsItem::calcualteBezierEndSlope(const QPointF& p0, const QPointF& p1, const QPointF& p2, const QPointF& p3) const {
    qreal t = 0.96;
    return qAtan2((3 * (1 - t) * (1 - t) * (p1.y() - p0.y()) + 6 * (1 - t) * t * (p2.y() - p1.y()) + 3 * t * t * (p3.y() - p2.y())), (3 * (1 - t) * (1 - t) * (p1.x() - p0.x()) + 6 * (1 - t) * t * (p2.x() - p1.x()) + 3 * t * t * (p3.x() - p2.x())));
}
