#include "negui_line_graphics_item.h"
#include "negui_line_style.h"
#include "negui_bezier_adjust_handled_graphics_item.h"
#include <QtMath>

// MyLineGraphicsItem

MyLineGraphicsItem::MyLineGraphicsItem(const QLineF& line, QGraphicsItem *parent) : QGraphicsPathItem(parent) {
    setLine(line);
}

void MyLineGraphicsItem::updateStyle() {
    if (isSetStyle()) {
        // pen
        setPen(((MyLineStyle*)style())->pen());
        
        // line
        setLine(_line);
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

const QPointF MyLineGraphicsItem::getStartPoint() {
    return _line.p1();
}

void MyLineGraphicsItem::setControlPoints(const QPointF& controlPoint1, const QPointF& controlPoint2) {
    if (isSetStyle()) {
        ((MyLineStyle*)style())->setRelativeP1(QPointF(100.0 * (controlPoint1.x() - _line.p1().x()) / (_line.p2() - _line.p1()).x(), 100.0 * (controlPoint1.y() - _line.p1().y()) / (_line.p2() - _line.p1()).y()));
        ((MyLineStyle*)style())->setRelativeP2(QPointF(100.0 * (controlPoint2.x() - _line.p2().x()) / (_line.p2() - _line.p1()).x(), 100.0 * (controlPoint2.y() - _line.p2().y()) / (_line.p2() - _line.p1()).y()));
    }
    
    updateStyle();
    emit lineControlPointAreUpdated();
}

const QPointF MyLineGraphicsItem::getControlPoint1() {
    QPointF controlPoint1 = _line.p1();
    if (isSetStyle())
        controlPoint1 += QPointF(0.01 * ((MyLineStyle*)style())->relativeP1().x() * (_line.p2() - _line.p1()).x(), 0.01 * ((MyLineStyle*)style())->relativeP1().y() * (_line.p2() - _line.p1()).y());
    
    return controlPoint1;
}

const QPointF MyLineGraphicsItem::getControlPoint2() {
    QPointF controlPoint2 = _line.p2();
    if (isSetStyle())
        controlPoint2 += QPointF(0.01 * ((MyLineStyle*)style())->relativeP2().x() * (_line.p2() - _line.p1()).x(), 0.01 * ((MyLineStyle*)style())->relativeP2().y() * (_line.p2() - _line.p1()).y());
    
    return controlPoint2;
}

const QPointF MyLineGraphicsItem::getEndPoint() {
    return _line.p2();
}

const qreal MyLineGraphicsItem::getEndSlope() {
    QPointF p0 = getStartPoint();
    QPointF p1 = getControlPoint1();
    QPointF p2 = getControlPoint2();
    QPointF p3 = getEndPoint();
    qreal t = 0.96;
    return qAtan2((3 * (1 - t) * (1 - t) * (p1.y() - p0.y()) + 6 * (1 - t) * t * (p2.y() - p1.y()) + 3 * t * t * (p3.y() - p2.y())), (3 * (1 - t) * (1 - t) * (p1.x() - p0.x()) + 6 * (1 - t) * t * (p2.x() - p1.x()) + 3 * t * t * (p3.x() - p2.x())));
}

QRectF MyLineGraphicsItem::getExtents() {
    return path().boundingRect();
}

QGraphicsItem* MyLineGraphicsItem::getFocusedGraphicsItem() {
    MyBezierAdjustHandledGraphicsItems* focusedGraphicsItems = new MyBezierAdjustHandledGraphicsItems(getStartPoint(), getControlPoint1(), getControlPoint2(), getEndPoint(), zValue());
    connect(focusedGraphicsItems, SIGNAL(controlPointsAreUpdated(const QPointF&, const QPointF&)), this, SLOT(setControlPoints(const QPointF&, const QPointF&)));
    
    return focusedGraphicsItems;
}

void MyLineGraphicsItem::setZValue(qreal z) {
    QGraphicsItem::setZValue(z);
};

void MyLineGraphicsItem::setLine(const QLineF& line) {
    _line = line;
    QPainterPath path;
    path.moveTo(getStartPoint());
    path.cubicTo(getControlPoint1(), getControlPoint2(), getEndPoint());
    setPath(path);
}
