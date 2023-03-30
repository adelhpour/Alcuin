#include "negui_polygon_style.h"
#include <QtMath>
#include <QJsonObject>
#include <QJsonArray>

// MyPolygonStyleBase

MyPolygonStyleBase::MyPolygonStyleBase(const QString& name) : My2DShapeStyleBase(name) {
    // stroke-width
    _parameters.push_back(new MyStrokeWidthParameter());
    
    // stroke
    _parameters.push_back(new MyStrokeParameter());
    
    // fill
    _parameters.push_back(new MyFillParameter());
    
    reset();
}

MyShapeStyleBase::SHAPE_STYLE MyPolygonStyleBase::type() {
    return POLYGON_SHAPE_STYLE;
}

const QList<MyAbsolutePointParameter*> MyPolygonStyleBase::pointParameters() const {
    QList<MyAbsolutePointParameter*> pointParameters;
    for (MyParameterBase* parameter : qAsConst(parameters())) {
        if (parameter->type() == MyParameterBase::POINT_PARAMETER_TYPE)
            pointParameters.push_back((MyAbsolutePointParameter*)parameter);
    }
    
    return pointParameters;
}

const QList<QPointF> MyPolygonStyleBase::points() const {
    QList<MyAbsolutePointParameter*> parameters = pointParameters();
    QList<QPointF> points;
    for (MyAbsolutePointParameter* parameter : qAsConst(parameters))
        points.push_back(QPointF(parameter->defaultValueX(), parameter->defaultValueY()));
    
    return points;
}

const QRectF MyPolygonStyleBase::getShapeExtents() {
    QRectF extents;
    extents.setX(INT_MAX);
    extents.setY(INT_MAX);
    QList<MyAbsolutePointParameter*> parameters = pointParameters();
    for (MyAbsolutePointParameter* parameter : qAsConst(parameters)) {
        if (parameter->defaultValueX() < extents.x())
            extents.setX(parameter->defaultValueX());
        if (parameter->defaultValueY() < extents.y())
            extents.setY(parameter->defaultValueY());
        if (extents.x() + extents.width() < parameter->defaultValueX())
            extents.setWidth(parameter->defaultValueX() - extents.x());
        if (extents.y() + extents.height() < parameter->defaultValueY())
            extents.setHeight(parameter->defaultValueY() - extents.y());
    }
    
    return extents;
}

void MyPolygonStyleBase::scaleToExtents(const QRectF& extents) {
    QRectF polygonBoundingRect = getShapeExtents();
    qreal xScaleFactor = extents.width() / polygonBoundingRect.width();
    qreal yScaleFactor = extents.height() / polygonBoundingRect.height();
    
    QList<MyAbsolutePointParameter*> parameters = pointParameters();
    for (MyAbsolutePointParameter* parameter : qAsConst(parameters)) {
        parameter->setDefaultValueX(extents.x() + xScaleFactor * (parameter->defaultValueX() - polygonBoundingRect.center().x()) + polygonBoundingRect.center().x());
        parameter->setDefaultValueY(extents.y() + yScaleFactor * (parameter->defaultValueY() - polygonBoundingRect.center().y()) + polygonBoundingRect.center().y());
    }
}

void MyPolygonStyleBase::moveBy(qreal dx, qreal dy) {
    QList<MyAbsolutePointParameter*> parameters = pointParameters();
    for (MyAbsolutePointParameter* parameter : qAsConst(parameters)) {
        parameter->setDefaultValueX(parameter->defaultValueX() + dx);
        parameter->setDefaultValueY(parameter->defaultValueY() + dy);
    }
}

void MyPolygonStyleBase::reset() {
    MyShapeStyleBase::reset();
    for (MyParameterBase* parameter : qAsConst(parameters())) {
        if (parameter->type() == MyParameterBase::POINT_PARAMETER_TYPE)
            _parameters.removeOne(parameter);
    }
}

void MyPolygonStyleBase::read(const QJsonObject &json) {
    My2DShapeStyleBase::read(json);
    MyParameterBase* parameter = NULL;
    
    // points
    if (json.contains("points") && json["points"].isArray()) {
        QJsonArray pointsArray = json["points"].toArray();
        MyAbsolutePointParameter* point = NULL;
        for (int pointIndex = 0; pointIndex < pointsArray.size(); ++pointIndex) {
            if (pointsArray[pointIndex].isObject()) {
                QJsonObject pointObject = pointsArray[pointIndex].toObject();
                point = new MyAbsolutePointParameter("point" + QString::number(pointIndex));
                // x
                if (pointObject.contains("x") && pointObject["x"].isDouble())
                    point->setDefaultValueX(pointObject["x"].toDouble());
                // y
                if (pointObject.contains("y") && pointObject["y"].isDouble())
                    point->setDefaultValueY(pointObject["y"].toDouble());
                _parameters.push_back(point);
            }
        }
    }
    else
        addDefaultPoints();
}

void MyPolygonStyleBase::write(QJsonObject &json) {
    My2DShapeStyleBase::write(json);
    MyParameterBase* parameter = NULL;
    
    // points
    QJsonArray pointsArray;
    for (MyParameterBase* parameter : qAsConst(parameters())) {
        if (parameter->type() == MyParameterBase::POINT_PARAMETER_TYPE) {
            QJsonObject pointObject;
            pointObject["x"] = ((MyAbsolutePointParameter*)parameter)->defaultValueX();
            pointObject["y"] = ((MyAbsolutePointParameter*)parameter)->defaultValueY();
            pointsArray.append(pointObject);
        }
    }
    json["points"] = pointsArray;
}

// MyNodePolygonStyle


MyNodePolygonStyle::MyNodePolygonStyle(const QString& name) : MyPolygonStyleBase(name) {
    
}

void MyNodePolygonStyle::addDefaultPoints() {
    MyAbsolutePointParameter* point = NULL;
    point = new MyAbsolutePointParameter("point1");
    point->setDefaultValueX(-25.0 * qCos(qDegreesToRadians(30.0)));
    point->setDefaultValueY(-25.0 * qSin(qDegreesToRadians(30.0)));
    _parameters.push_back(point);
    
    point = new MyAbsolutePointParameter("point2");
    point->setDefaultValueX(0.0);
    point->setDefaultValueY(25.0);
    _parameters.push_back(point);
    
    point = new MyAbsolutePointParameter("point3");
    point->setDefaultValueX(25.0 * qCos(qDegreesToRadians(30.0)));
    point->setDefaultValueY(-25.0 * qSin(qDegreesToRadians(30.0)));
    _parameters.push_back(point);
    
    point = new MyAbsolutePointParameter("point4");
    point->setDefaultValueX(-25.0 * qCos(qDegreesToRadians(30.0)));
    point->setDefaultValueY(-25.0 * qSin(qDegreesToRadians(30.0)));
    _parameters.push_back(point);
}

// MyArrowHeadPolygonStyle

MyArrowHeadPolygonStyle::MyArrowHeadPolygonStyle(const QString& name) : MyPolygonStyleBase(name) {
    
}

void MyArrowHeadPolygonStyle::addDefaultPoints() {
    MyAbsolutePointParameter* point = NULL;
    point = new MyAbsolutePointParameter("point1");
    point->setDefaultValueX(-10.0);
    point->setDefaultValueY(-5.0);
    _parameters.push_back(point);
    
    point = new MyAbsolutePointParameter("point2");
    point->setDefaultValueX(0.0);
    point->setDefaultValueY(0.0);
    _parameters.push_back(point);
    
    point = new MyAbsolutePointParameter("point3");
    point->setDefaultValueX(-10.0);
    point->setDefaultValueY(5.0);
    _parameters.push_back(point);
    
    point = new MyAbsolutePointParameter("point4");
    point->setDefaultValueX(-10.0);
    point->setDefaultValueY(-5.0);
    _parameters.push_back(point);
}
