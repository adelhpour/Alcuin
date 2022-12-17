#include "negui_arrow_head.h"
#include "negui_edge.h"
#include "negui_element_graphics_item_builder.h"
#include "negui_arrow_head_graphics_item.h"
#include <QJsonObject>

// MyArrowHead

MyArrowHead::MyArrowHead(const QString& name, MyElementBase* edge) : MyElementBase(name) {
    _edge = edge;
    _position = QPointF(0.0, 0.0);
    _slope = 0.0;
    _graphicsItem = createArrowHeadSceneGraphicsItem();
    connect(_graphicsItem, &MyElementGraphicsItemBase::mouseLeftButtonIsPressed, this, [this] () { emit elementObject(this); });
    connect(_graphicsItem, SIGNAL(mouseLeftButtonIsDoubleClicked()), this, SLOT(displayFeatureMenu()));
}

MyArrowHead::~MyArrowHead() {
    delete _graphicsItem;
}

MyArrowHead::ELEMENT_TYPE MyArrowHead::type() {
    return ARROW_HEAD_ELEMENT;
};

MyElementBase* MyArrowHead::edge() {
    return _edge;
}

void MyArrowHead::updateGraphicsItem() {
    MyElementBase::updateGraphicsItem();
    updatePlacement(_position, _slope);
}

void MyArrowHead::updatePlacement(const QPointF& position, const qreal& slope) {
    _position = position;
    _slope = slope;
    ((MyArrowHeadSceneGraphicsItem*)graphicsItem())->update(_position, _slope);
    graphicsItem()->setZValue(calculateZValue());
}

void MyArrowHead::setSelected(const bool& selected) {
    graphicsItem()->setSelectedWithFill(selected);
    
    if (!selected)
        graphicsItem()->setSelectedWithStroke(selected);
}

const QRectF MyArrowHead::getExtents() {
    return QRectF(0.0, 0.0, 0.0, 0.0);
}

QWidget* MyArrowHead::getFeatureMenu() {
    QWidget* featureMenu = MyElementBase::getFeatureMenu();
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();
    
    // edge
    contentLayout->addWidget(new MyLabel("Edge"), contentLayout->rowCount(), 0);
    contentLayout->addWidget(new MyReadOnlyLineEdit(edge()->name()), contentLayout->rowCount() - 1, 1);
    
    // spacer
    QLayoutItem* spacerItem = new MySpacerItem(0, 10);
    contentLayout->addItem(spacerItem, contentLayout->rowCount(), 0, 1, 2);
    
    // add remove buttons
    connect(style()->getAddRemoveShapeStylesButtons(), SIGNAL(askForAddShapeStyle(MyShapeStyleBase*)), featureMenu, SIGNAL(askForAddShapeStyle(MyShapeStyleBase*)));
    connect(style()->getAddRemoveShapeStylesButtons(), SIGNAL(askForRemoveShapeStyle(MyShapeStyleBase*)), featureMenu, SIGNAL(askForRemoveShapeStyle(MyShapeStyleBase*)));
    connect(featureMenu, SIGNAL(askForSetRemovingMenu(QList<MyShapeStyleBase*>)), style()->getAddRemoveShapeStylesButtons(), SLOT(setRemovingMenu(QList<MyShapeStyleBase*>)));
    contentLayout->addWidget(style()->getAddRemoveShapeStylesButtons(), contentLayout->rowCount(), 1);
    
    return featureMenu;
}

const qint32 MyArrowHead::calculateZValue() {
    return edge()->calculateZValue() + 1;
}

void MyArrowHead::read(const QJsonObject &json) {
    
}

void MyArrowHead::write(QJsonObject &json) {
    
}
