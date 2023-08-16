#ifndef __NEGUI_SHAPE_GRAPHICS_ITEM_BUILDER_H
#define __NEGUI_SHAPE_GRAPHICS_ITEM_BUILDER_H

#include "negui_shape_graphics_item_base.h"
#include <QGraphicsItem>

MyShapeGraphicsItemBase* createEllipseShape(qreal x, qreal y, QGraphicsItem *parent);

MyShapeGraphicsItemBase* createRectShape(qreal x, qreal y, QGraphicsItem *parent);

MyShapeGraphicsItemBase* createPolygonShape(qreal x, qreal y, QGraphicsItem *parent);

MyShapeGraphicsItemBase* createClassicLineShape(const QLineF& line, QGraphicsItem *parent);

MyShapeGraphicsItemBase* createConnectedToStartCentroidShapeLineShape(const QLineF& line, QGraphicsItem *parent);

MyShapeGraphicsItemBase* createConnectedToEndCentroidShapeLineShape(const QLineF& line, QGraphicsItem *parent);

MyShapeGraphicsItemBase* createTextShape(qreal x, qreal y, const QString& elementName, QGraphicsItem *parent);

MyShapeGraphicsItemBase* createCentroidShape(qreal x, qreal y, QGraphicsItem *parent);

#endif
