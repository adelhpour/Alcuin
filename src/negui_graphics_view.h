#ifndef __NEGUI_GRAPHICS_VIEW_H
#define __NEGUI_GRAPHICS_VIEW_H

#include <QGraphicsView>
#include <QToolButton>
#include <QWheelEvent>
#include <QPrinter>
#include <QProxyStyle>

class MyGraphicsView : public QGraphicsView {
    Q_OBJECT
    
public:
    
    MyGraphicsView(QWidget* parent = nullptr);
    
    const qreal currentScale() const;
    void setMinScale(const qreal& minScale);
    void setMaxScale(const qreal& maxScale);
    const qreal minScale() const;
    const qreal maxScale() const;
    void animatedScale(const qint32& delta);

    QToolButton* getZoomInButton();
    QToolButton* getZoomOutButton();
    QToolButton* createZoomInMenuButton();
    QToolButton* createZoomOutMenuButton();
    
signals:
    void enterKeyIsPressed();
    void askForDisplayContextMenu(const QPointF& position);

public slots:

    void zoomIn();
    void zoomOut();
    
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
    
    qint32 _numScheduledScalings;
    qreal _minScale;
    qreal _maxScale;
    qint32 _panStartX;
    qint32 _panStartY;
    bool _panMode;
    bool _isPanned;
};

class MyProxyStyle : public QProxyStyle {
    
public:
    
    MyProxyStyle(QStyle *style = nullptr);

    int styleHint(StyleHint hint, const QStyleOption* option = nullptr, const QWidget* widget = nullptr, QStyleHintReturn* returnData = nullptr) const override;
};

#endif
