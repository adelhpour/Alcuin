#include "negui_arrow_head.h"
#include "negui_edge.h"
#include "negui_arrow_head_style.h"
#include "negui_arrow_head_graphics_item_builder.h"
#include "negui_arrow_head_graphics_item.h"
#include "negui_customized_common_widgets.h"


#include <QJsonObject>
#include <QGridLayout>

// MyArrowHeadBase

MyArrowHeadBase::MyArrowHeadBase(const QString& name, MyElementBase* edge) : MyElementBase(name) {
    _edge = edge;
    _position = QPointF(0.0, 0.0);
    _slope = 0.0;
    _graphicsItem = createArrowHeadSceneGraphicsItem();
    connect(_graphicsItem, &MyElementGraphicsItemBase::mouseLeftButtonIsPressed, this, [this] () { emit elementObject(this); });
    connect(_graphicsItem, SIGNAL(mouseLeftButtonIsDoubleClicked()), this, SLOT(createFeatureMenu()));
    connect(_graphicsItem, SIGNAL(askForCreateChangeStageCommand()), this, SIGNAL(askForCreateChangeStageCommand()));
}

MyArrowHeadBase::~MyArrowHeadBase() {
    delete _graphicsItem;
}

MyArrowHeadBase::ELEMENT_TYPE MyArrowHeadBase::type() {
    return ARROW_HEAD_ELEMENT;
};

MyElementBase* MyArrowHeadBase::edge() {
    return _edge;
}

void MyArrowHeadBase::updateGraphicsItem() {
    MyElementBase::updateGraphicsItem();
    updatePlacement(_position, _slope);
}

void MyArrowHeadBase::updatePlacement(const QPointF& position, const qreal& slope) {
    _position = position;
    _slope = slope;
    ((MyArrowHeadSceneGraphicsItem*)graphicsItem())->update(_position, _slope);
    graphicsItem()->setZValue(calculateZValue());
}

void MyArrowHeadBase::setSelected(const bool& selected) {
    MyElementBase::setSelected(selected);
    graphicsItem()->setSelectedWithStroke(selected);
    
    if (!selected)
        graphicsItem()->setSelectedWithFill(selected);
}

const QRectF MyArrowHeadBase::getExtents() {
    return QRectF(0.0, 0.0, 0.0, 0.0);
}

QWidget* MyArrowHeadBase::getFeatureMenu() {
    QWidget* featureMenu = MyElementBase::getFeatureMenu();
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();
    
    // edge
    contentLayout->addWidget(new MyLabel("Edge"), contentLayout->rowCount(), 0);
    contentLayout->addWidget(new MyReadOnlyLineEdit(edge()->name()), contentLayout->rowCount() - 1, 1);
    
    // spacer
    QLayoutItem* spacerItem = new MySpacerItem(0, 10);
    contentLayout->addItem(spacerItem, contentLayout->rowCount(), 0, 1, 2);

    return featureMenu;
}

const qint32 MyArrowHeadBase::calculateZValue() {
    return edge()->calculateZValue() + 1;
}

void MyArrowHeadBase::read(const QJsonObject &json) {
    
}

void MyArrowHeadBase::write(QJsonObject &json) {
    
}

// MyClassicArrowHead

MyClassicArrowHead::MyClassicArrowHead(const QString& name, MyElementBase* edge) : MyArrowHeadBase(name, edge) {

}

MyArrowHeadBase::ARROW_HEAD_TYPE MyClassicArrowHead::arrowHeadType() {
    return CLASSIC_ARROW_HEAD;
}

QWidget* MyClassicArrowHead::getFeatureMenu() {
    QWidget* featureMenu = MyArrowHeadBase::getFeatureMenu();
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();

    // add remove buttons
    QWidget* addRemoveShapeStylesButtons = ((MyClassicArrowHeadStyle*)style())->addRemoveShapeStylesButtons();
    connect(addRemoveShapeStylesButtons, SIGNAL(askForAddShapeStyle(MyShapeStyleBase*)), featureMenu, SIGNAL(askForAddShapeStyle(MyShapeStyleBase*)));
    connect(addRemoveShapeStylesButtons, SIGNAL(askForRemoveShapeStyle(MyShapeStyleBase*)), featureMenu, SIGNAL(askForRemoveShapeStyle(MyShapeStyleBase*)));
    connect(featureMenu, SIGNAL(askForSetRemovingMenu(QList<MyShapeStyleBase*>)), addRemoveShapeStylesButtons, SLOT(setRemovingMenu(QList<MyShapeStyleBase*>)));
    contentLayout->addWidget(addRemoveShapeStylesButtons, contentLayout->rowCount(), 1);

    return featureMenu;
}
