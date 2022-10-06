#ifndef __NEGUI_GRAPHICS_VIEW_H
#define __NEGUI_GRAPHICS_VIEW_H

#include "negui_base.h"
#include "negui_graphics_scene.h"

#include <QGraphicsView>
#include <QScrollbar>
#include <QWheelEvent>
#include <QTimeLine>

class MyGraphicsScene;

class MyGraphicsView : public QGraphicsView {
    Q_OBJECT
    
public:
    
    MyGraphicsView(QWidget* parent = nullptr);
    
    MyGraphicsScene* scene();
    const qreal currentScale() const;
    void setMinScale(const qreal& minScale);
    void setMaxScale(const qreal& maxScale);
    const qreal minScale() const;
    const qreal maxScale() const;
    
signals:
    void enterKeyIsPressed();
    
private slots:
    
    void setToolTip(const QString& toolTip);
    void exportFigure(const QString& fileName, QPrinter::OutputFormat outputFormat);
    void resetScale();
    void scalingTime(qreal x);
    void animFinished();
    
protected:
    
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    
    int _numScheduledScalings;
    qreal _minScale;
    qreal _maxScale;
    int _panStartX;
    int _panStartY;
    bool _panMode;
};

#endif
