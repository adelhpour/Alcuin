#include "negui_graphics_view.h"
#include "negui_graphics_scene.h"
#include "negui_customized_common_widgets.h"
#include "negui_menu_button_builder.h"

#include <QScrollbar>
#include <QPrinter>
#include <QSvgGenerator>
#include <QFileInfo>

// MyGraphicsView

MyGraphicsView::MyGraphicsView(QWidget* parent) : QGraphicsView(parent) {
    setStyleSheet("QGraphicsView {background-color : white; border-radius: 10px;}");
    setContentsMargins(0, 0, 0, 0);
    setStyle(new MyProxyStyle(style()));
    setMouseTracking(true);
    
    _minScale = 1.5 / 10.0;
    _maxScale = 10.0;
    _numScheduledScalings = 0;
    _panMode = false;
    _isPanned = false;
    
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setScene(new MyGraphicsScene(this));
    setSceneRect(scene()->sceneRect().x(), scene()->sceneRect().y(), scene()->sceneRect().width(), scene()->sceneRect().height());
    connect(this, SIGNAL(askForDisplayContextMenu(const qreal&, const qreal&)), scene(), SLOT(displayContextMenu(const qreal&, const qreal&)));
    connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateFrame()));
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateFrame()));

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

void MyGraphicsView::saveFigure(const QString& fileName) {
    QString fileExtension = QFileInfo(fileName).completeSuffix();
    if (fileExtension == "pdf")
        saveFigureAsPDF(fileName, mapToScene(viewport()->geometry()).boundingRect());
    else if (fileExtension == "svg")
        saveFigureAsSVG(fileName, mapToScene(viewport()->geometry()).boundingRect());
}

void MyGraphicsView::saveFigureAsPDF(const QString& fileName, const QRectF& pageRect) {
    QPrinter printer(QPrinter::ScreenResolution);
    printer.setPageMargins(QMarginsF(0.0, 0.0, 0.0, 0.0));
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QSize(pageRect.width(), pageRect.height()), QPageSize::Point));
    QPainter painter(&printer);
    scene()->render(&painter, QRectF(), QRectF(pageRect.x() + 10, pageRect.y() + 10, pageRect.width() - 20, pageRect.height() - 20));
}

void MyGraphicsView::saveFigureAsSVG(const QString& fileName, const QRectF& pageRect) {
    QSvgGenerator svgGenerator;
    svgGenerator.setFileName(fileName);
    svgGenerator.setSize(QSize(pageRect.width(), pageRect.height()));
    svgGenerator.setViewBox(QRectF(0, 0, pageRect.width(), pageRect.height()));
    QPainter painter;
    painter.begin(&svgGenerator);
    scene()->render(&painter, QRectF(), pageRect);
    painter.end();
}

void MyGraphicsView::resetScale() {
    resetTransform();
    centerOn(sceneRect().center());
    scaleChanged(currentScale());
    updateFrame();
}

void MyGraphicsView::zoomIn() {
    zoom(0.1);
}

void MyGraphicsView::zoomOut() {
    zoom(-0.1);
}

QList<QAbstractButton*> MyGraphicsView::getToolbarMenuButtons(const QString& iconsDirectoryPath) {
    return createToolbarMenuButtons(this, iconsDirectoryPath);
}

void MyGraphicsView::zoom(const qreal factor) {
    QTransform transformMatrix = transform();
    transformMatrix.setMatrix(qMin(qMax(transform().m11() + factor, minScale()), maxScale()), transform().m12(), transform().m13(),
                              transform().m21(), qMin(qMax(transform().m22() + factor, minScale()), maxScale()), transform().m23(),
                              transform().m31(), transform().m32(), transform().m33());
    setTransform(transformMatrix);
    emit scaleChanged(currentScale());
    updateFrame();
}

void MyGraphicsView::updateFrame() {
    ((MyGraphicsScene*)scene())->updateViewFrame(mapToScene(viewport()->geometry()).boundingRect());
}

void MyGraphicsView::updateFrame(const QRectF& frameRect) {
    ((MyGraphicsScene*)scene())->updateViewFrame(frameRect);
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
                emit askForDisplayContextMenu(event->globalPos().x(), event->globalPos().y());
        }
    }
    _isPanned = false;
    _panMode = false;
}

void MyGraphicsView::leaveEvent(QEvent *event) {
    emit mouseLeft();
    QGraphicsView::leaveEvent(event);
}

void MyGraphicsView::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    updateFrame();
}

// MyProxyStyle

MyProxyStyle::MyProxyStyle(QStyle *style) : QProxyStyle(style) {
    
}

int MyProxyStyle::styleHint(StyleHint hint, const QStyleOption* option, const QWidget* widget, QStyleHintReturn* returnData) const {
    if (hint == QStyle::SH_ToolTip_WakeUpDelay)
        return 250;
    return QProxyStyle::styleHint(hint, option, widget, returnData);
}