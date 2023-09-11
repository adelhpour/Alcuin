#include "negui_graphics_view.h"
#include "negui_graphics_scene.h"
#include "negui_customized_common_widgets.h"

#include <QScrollbar>
#include <QPrinter>
#include <QSvgGenerator>

// MyGraphicsView

MyGraphicsView::MyGraphicsView(QWidget* parent) : QGraphicsView(parent) {
    setStyleSheet("QGraphicsView {background-color : white; border-radius: 10px;}");
    setContentsMargins(0, 0, 0, 0);
    setStyle(new MyProxyStyle(style()));
    setMouseTracking(true);
    
    _minScale = 1.0 / 10.0;
    _maxScale = 10.0;
    _numScheduledScalings = 0;
    _panMode = false;
    _isPanned = false;
    
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setScene(new MyGraphicsScene(this));
    setSceneRect(scene()->sceneRect().x(), scene()->sceneRect().y(), scene()->sceneRect().width(), scene()->sceneRect().height());
    connect(this, SIGNAL(askForDisplayContextMenu(const QPointF&)), scene(), SLOT(displayContextMenu(const QPointF&)));
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    resetScale();
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

void MyGraphicsView::exportFigure(const QString& fileName, const QString& fileExtension) {
    qreal pageX = scene()->sceneRect().x() + (0.5 / currentScale()) * (currentScale() - 1) * scene()->sceneRect().width();
    qreal pageY = scene()->sceneRect().y() + (0.5 / currentScale()) * (currentScale() - 1) * scene()->sceneRect().height();
    qreal pageWidth = scene()->sceneRect().width() / currentScale();
    qreal pageHeight = scene()->sceneRect().height() / currentScale();
    if (fileExtension == "pdf")
        exportFigureAsPDF(fileName, QRectF(pageX, pageY, pageWidth, pageHeight));
    else if (fileExtension == "svg")
        exportFigureAsSVG(fileName, QRectF(pageX, pageY, pageWidth, pageHeight));
}

void MyGraphicsView::exportFigureAsPDF(const QString& fileName, const QRectF& pageRect) {
    QPrinter printer(QPrinter::ScreenResolution);
    printer.setPageMargins(QMarginsF(0.0, 0.0, 0.0, 0.0));
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QSize(scene()->sceneRect().width(), scene()->sceneRect().height()), QPageSize::Point));
    QPainter painter(&printer);
    scene()->render(&painter, scene()->sceneRect(), pageRect);
}

void MyGraphicsView::exportFigureAsSVG(const QString& fileName, const QRectF& pageRect) {
    QSvgGenerator svgGenerator;
    svgGenerator.setFileName(fileName);
    svgGenerator.setSize(QSize(scene()->sceneRect().width(), scene()->sceneRect().height()));
    svgGenerator.setViewBox(pageRect);
    QPainter painter;
    painter.begin(&svgGenerator);
    scene()->render(&painter, scene()->sceneRect(), pageRect);
    painter.end();
}

void MyGraphicsView::resetScale() {
    resetTransform();
}

void MyGraphicsView::zoomIn() {
    zoom(0.1);
}

void MyGraphicsView::zoomOut() {
    zoom(-0.1);
}

QToolButton* MyGraphicsView::getZoomInButton() {
    return createZoomInMenuButton();
}

QToolButton* MyGraphicsView::getZoomOutButton() {
    return createZoomOutMenuButton();
}

QToolButton* MyGraphicsView::createZoomInMenuButton() {
    MyToolButton* button = new MyToolButton();
    connect(button, &QToolButton::clicked, this, &MyGraphicsView::zoomIn);
    return button;
}

QToolButton* MyGraphicsView::createZoomOutMenuButton() {
    MyToolButton* button = new MyToolButton();
    connect(button, &QToolButton::clicked, this, &MyGraphicsView::zoomOut);
    return button;
}

void MyGraphicsView::zoom(const qreal factor) {
    QTransform transformMatrix = transform();
    transformMatrix.setMatrix(qMin(qMax(transform().m11() + factor, minScale()), maxScale()), transform().m12(), transform().m13(),
                              transform().m21(), qMin(qMax(transform().m22() + factor, minScale()), maxScale()), transform().m23(),
                              transform().m31(), transform().m32(), transform().m33());
    setTransform(transformMatrix);
    emit scaleChanged(currentScale());
}

void MyGraphicsView::wheelEvent(QWheelEvent * event) {
    qint32 zoomSpeedFactor = 1;
    if (currentScale() > 1.000)
        zoomSpeedFactor = qMin(qMax(qAbs(event->delta()) / 100, 1), 10);
    if (event->delta() > 0.0001)
        zoom(zoomSpeedFactor * 0.01);
    else if (event->delta() < -0.0001)
        zoom(zoomSpeedFactor * -0.01);
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event) {
    QGraphicsView::mousePressEvent(event);
    if (!event->isAccepted()) {
        if (event->button() == Qt::RightButton) {
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
        _isPanned = true;
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - _panStartX));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - _panStartY));
        _panStartX = event->x();
        _panStartY = event->y();
        event->accept();
    }
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    QGraphicsView::mouseReleaseEvent(event);
    if (!((MyGraphicsScene*)scene())->whetherMouseReleaseEventIsAccepted()) {
        if (event->button() == Qt::RightButton) {
            if (!_isPanned)
                emit askForDisplayContextMenu(event->globalPos());
        }
    }
    _isPanned = false;
    _panMode = false;
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

void MyGraphicsView::leaveEvent(QEvent *event) {
    emit mouseLeft();
    QGraphicsView::leaveEvent(event);
}

// MyProxyStyle

MyProxyStyle::MyProxyStyle(QStyle *style) : QProxyStyle(style) {
    
}

int MyProxyStyle::styleHint(StyleHint hint, const QStyleOption* option, const QWidget* widget, QStyleHintReturn* returnData) const {
    if (hint == QStyle::SH_ToolTip_WakeUpDelay)
        return 250;
    return QProxyStyle::styleHint(hint, option, widget, returnData);
}