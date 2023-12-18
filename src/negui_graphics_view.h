#ifndef __NEGUI_GRAPHICS_VIEW_H
#define __NEGUI_GRAPHICS_VIEW_H

#include <QGraphicsView>
#include <QToolButton>
#include <QWheelEvent>
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

    QList<QAbstractButton*> getToolbarMenuButtons(const QString& iconsDirectoryPath);

    void saveFigureAsPDF(const QString& fileName, const QRectF& pageRect);
    void saveFigureAsSVG(const QString& fileName ,const QRectF& pageRect);
    
signals:

    void askForDisplayContextMenu(const qreal&, const qreal&);
    void mouseLeft();
    void scaleChanged(const qreal);

public slots:

    void zoomIn();
    void zoomOut();
    void updateFrame();
    void updateFrame(const QRectF& frameRect);
    
private slots:
    
    void setToolTip(const QString& toolTip);
    void saveFigure(const QString& fileName);
    void resetScale();
    void zoom(const qreal factor);
    
protected:
    
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

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
