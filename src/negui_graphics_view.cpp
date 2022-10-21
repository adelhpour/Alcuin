#include "negui_graphics_view.h"
#include "negui_graphics_scene.h"
#include <QScrollbar>
#include <QTimeLine>

// MyGraphicsView

MyGraphicsView::MyGraphicsView(QWidget* parent) : QGraphicsView(parent) {
    setStyleSheet("QGraphicsView {background-color : white; border-radius: 10px;}");
    setContentsMargins(0, 0, 0, 0);
    setStyle(new MyProxyStyle(style()));
    
    _minScale = 1.0 / 3.0;
    _maxScale = 3.0;
    _numScheduledScalings = 0;
    _panMode = false;
    
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setGeometry(75, 50, 900, 600);
    setScene(new MyGraphicsScene(this));
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
};

const qreal MyGraphicsView::currentScale() const {
    return transform().m11();
}

void MyGraphicsView::setMinScale(const qreal& minScale) {
    _minScale = minScale;
}

void MyGraphicsView::setMaxScale(const qreal& maxScale) {
    _maxScale = maxScale;
}

const qreal MyGraphicsView::minScale() const {
    return _minScale;
}

const qreal MyGraphicsView::maxScale() const {
    return _maxScale;
}

void MyGraphicsView::setToolTip(const QString& toolTip) {
    QWidget::setToolTip(toolTip);
}

void MyGraphicsView::exportFigure(const QString& fileName, QPrinter::OutputFormat outputFormat) {
    qreal pageX = scene()->sceneRect().x() + (0.5 / currentScale()) * (currentScale() - 1) * scene()->sceneRect().width();
    qreal pageY = scene()->sceneRect().y() + (0.5 / currentScale()) * (currentScale() - 1) * scene()->sceneRect().height();
    qreal pageWidth = scene()->sceneRect().width() / currentScale();
    qreal pageHeight = scene()->sceneRect().height() / currentScale();
    
    QPrinter printer(QPrinter::ScreenResolution);
    printer.setPageMargins(QMarginsF(0.0, 0.0, 0.0, 0.0));
    printer.setOutputFormat(outputFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QSize(scene()->sceneRect().width(), scene()->sceneRect().height()), QPageSize::Point));
    QPainter painter(&printer);
    scene()->render(&painter, scene()->sceneRect(), QRectF(pageX, pageY, pageWidth, pageHeight));
}

void MyGraphicsView::resetScale() {
    resetTransform();
}

void MyGraphicsView::scalingTime(qreal x) {
    qreal factor = 1.0 + qreal(_numScheduledScalings) / 10000.0;
    if ((factor  > 1.00000 && (currentScale() < _maxScale)) || (factor  < 1.00000 && (currentScale() > _minScale))) {
        scale(factor, factor);
    }
}

void MyGraphicsView::animFinished() {
    if (_numScheduledScalings > 0)
        _numScheduledScalings--;
    else
        _numScheduledScalings++;
    sender()->~QObject();
}

void MyGraphicsView::wheelEvent(QWheelEvent * event) {
    _numScheduledScalings = event->delta();
    
    QTimeLine* anim = new QTimeLine(10, this);
    anim->setUpdateInterval(1);
    
    connect(anim, SIGNAL(valueChanged(qreal)), SLOT(scalingTime(qreal)));
    connect(anim, SIGNAL(finished()), this, SLOT(animFinished()));
    anim->start();
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event) {
    QGraphicsView::mousePressEvent(event);
    if (!event->isAccepted()) {
        if (event->button() == Qt::LeftButton) {
            _panMode = true;
            _panStartX = event->x();
            _panStartY = event->y();
            event->accept();
        }
    }
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);
    if (_panMode) {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - _panStartX));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - _panStartY));
        _panStartX = event->x();
        _panStartY = event->y();
        event->accept();
    }
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    QGraphicsView::mouseReleaseEvent(event);
    if (_panMode) {
        if (event->button() == Qt::LeftButton) {
            _panMode = false;
            event->accept();
        }
    }
}

void MyGraphicsView::keyPressEvent(QKeyEvent *event) {
    QGraphicsView::keyPressEvent(event);
    if (!event->isAccepted()) {
        if (event->key() == Qt::Key_Return) {
            emit enterKeyIsPressed();
            event->accept();
        }
    }
}

// MyProxyStyle

MyProxyStyle::MyProxyStyle(QStyle *style) : QProxyStyle(style) {
    
}

int MyProxyStyle::styleHint(StyleHint hint, const QStyleOption* option, const QWidget* widget, QStyleHintReturn* returnData) const {
    if (hint == QStyle::SH_ToolTip_WakeUpDelay)
        return 250;
    return QProxyStyle::styleHint(hint, option, widget, returnData);
}
