#include "negui_secondary_graphics_view.h"
#include "negui_graphics_scene.h"

// MySecondaryGraphicsView

MySecondaryGraphicsView::MySecondaryGraphicsView(QGraphicsScene* mainScene, QWidget* parent) : QGraphicsView(parent) {
    setFrameShape(Box);
    setStyleSheet("QFrame {border: 1px solid gray; border-radius: 2px solid gray;}");
    setContentsMargins(0, 0, 0, 0);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setInteractive(false);
    setScene(mainScene);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setSceneRect(scene()->sceneRect());
    adjustScale();
    setFixedSize(200, 200);
};

void MySecondaryGraphicsView::adjustScale() {
    QTransform transformMatrix = transform();
    transformMatrix.setMatrix(transform().m11() - 0.9875, transform().m12(), transform().m13(),
                              transform().m21(), transform().m22() - 0.9875, transform().m23(),
                              transform().m31(), transform().m32(), transform().m33());
    setTransform(transformMatrix);
}

