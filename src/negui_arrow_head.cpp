#include "negui_arrow_head.h"
#include "negui_edge.h"
#include "negui_element_graphics_item_builder.h"
#include <QJsonObject>

// MyArrowHead

MyArrowHead::MyArrowHead(const QString& name, MyElementBase* edge) : MyElementBase(name) {
    _style = NULL;
    _edge = edge;
    _graphicsItem = createArrowHeadSceneGraphicsItem();
    connect(_graphicsItem, &MyElementGraphicsItemBase::mouseLeftButtonIsPressed, this, [this] () { emit elementObject(this); });
    connect(_graphicsItem, SIGNAL(askForElementFeatureMenu()), this, SLOT(getFeatureMenu()));
    connect(_graphicsItem, SIGNAL(askForSetShapeStyles(QList<MyShapeStyleBase*>)), this, SLOT(setShapeStyles(QList<MyShapeStyleBase*>)));
    enableNormalMode();
}

MyArrowHead::~MyArrowHead() {
    delete _graphicsItem;
}

MyArrowHead::ELEMENT_TYPE MyArrowHead::type() {
    return ARROW_HEAD_ELEMENT;
};

const QString MyArrowHead::typeAsString() const {
    return "ArrowHead";
}

MyElementBase* MyArrowHead::edge() {
    return _edge;
}

void MyArrowHead::setSelected(const bool& selected) {
    graphicsItem()->setSelectedWithFill(selected);
    
    if (!selected)
        graphicsItem()->setSelectedWithStroke(selected);
}

void MyArrowHead::setShapeStyles(QList<MyShapeStyleBase*> shapeStyles) {
    MyElementBase::setShapeStyles(shapeStyles);
    ((MyEdge*)edge())->updatePoints();
}

void MyArrowHead::enableNormalMode() {
    MyElementBase::enableNormalMode();
    graphicsItem()->setCursor(Qt::ArrowCursor);
}

void MyArrowHead::enableAddNodeMode() {
    graphicsItem()->setCursor(Qt::ArrowCursor);
}

void MyArrowHead::enableSelectNodeMode() {
    graphicsItem()->setCursor(Qt::ArrowCursor);
}

void MyArrowHead::enableAddEdgeMode() {
    graphicsItem()->setCursor(Qt::ArrowCursor);
}

void MyArrowHead::enableSelectEdgeMode() {
    graphicsItem()->setCursor(Qt::ArrowCursor);
}

void MyArrowHead::enableRemoveMode() {
    graphicsItem()->setCursor(Qt::ArrowCursor);
}

QWidget* MyArrowHead::getFeatureMenu() {
    QWidget* featureMenu = MyElementBase::getFeatureMenu();
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();
    
    // edge
    contentLayout->addWidget(new MyLabel("Edge"), contentLayout->rowCount(), 0);
    contentLayout->addWidget(new MyReadOnlyLineEdit(edge()->name()), contentLayout->rowCount() - 1, 1);
    
    return featureMenu;
}

const qint32 MyArrowHead::calculateZValue() {
    return edge()->calculateZValue() + 1;
}

void MyArrowHead::read(const QJsonObject &json) {
    
}

void MyArrowHead::write(QJsonObject &json) {
    
}
