#include "negui_arrow_head.h"
#include "negui_edge.h"
#include "negui_arrow_head_style.h"
#include "negui_arrow_head_graphics_item_builder.h"
#include "negui_arrow_head_graphics_item.h"
#include "negui_customized_common_widgets.h"


#include <QJsonObject>
#include <QGridLayout>

// MyArrowHeadBase

MyArrowHeadBase::MyArrowHeadBase(const QString& name, MyNetworkElementBase* edge) : MyNetworkElementBase(name) {
    _edge = edge;
    _position = QPointF(0.0, 0.0);
    _slope = 0.0;
}

MyArrowHeadBase::ELEMENT_TYPE MyArrowHeadBase::type() {
    return ARROW_HEAD_ELEMENT;
};

void MyArrowHeadBase::connectGraphicsItem() {
    MyNetworkElementBase::connectGraphicsItem();
}

MyNetworkElementBase* MyArrowHeadBase::edge() {
    return _edge;
}

void MyArrowHeadBase::updateGraphicsItem() {
    MyNetworkElementBase::updateGraphicsItem();
    updatePlacement(_position, _slope);
}

void MyArrowHeadBase::updatePlacement(const QPointF& position, const qreal& slope) {
    _position = position;
    _slope = slope;
    ((MyArrowHeadSceneGraphicsItem*)graphicsItem())->update(_position, _slope);
    graphicsItem()->setZValue(calculateZValue());
}

const bool MyArrowHeadBase::isCopyable() {
    if (edge()->isCopyable())
        return true;

    return false;
}

const bool MyArrowHeadBase::isCuttable() {
    if (edge()->isCopyable())
        return true;

    return false;
}

void MyArrowHeadBase::setSelectedWithColor(const bool& selected) {
    graphicsItem()->setSelectedWithBorderColor(selected);
    
    if (!selected)
        graphicsItem()->setSelectedWithFillColor(selected);
}

const bool MyArrowHeadBase::canBeMovedExternally() {
    return false;
}

void MyArrowHeadBase::moveExternally(const qreal& dx, const qreal& dy) {
    if (canBeMovedExternally())
        return;
}

const QRectF MyArrowHeadBase::getExtents() {
    return QRectF(0.0, 0.0, 0.0, 0.0);
}

QWidget* MyArrowHeadBase::getFeatureMenu() {
    QWidget* featureMenu = MyNetworkElementBase::getFeatureMenu();
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();
    
    // edge
    contentLayout->addWidget(new MyLabel("Edge"), contentLayout->rowCount(), 0, Qt::AlignLeft);
    contentLayout->addWidget(new MyReadOnlyLineEdit(edge()->name()), contentLayout->rowCount() - 1, 1, Qt::AlignRight);
    
    // spacer
    addSpacerItemToFeatureMenu(featureMenu);

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

MyClassicArrowHead::MyClassicArrowHead(const QString& name, MyNetworkElementBase* edge) : MyArrowHeadBase(name, edge) {
    _graphicsItem = createArrowHeadSceneGraphicsItem();
    connectGraphicsItem();
}

MyArrowHeadBase::ARROW_HEAD_TYPE MyClassicArrowHead::arrowHeadType() {
    return CLASSIC_ARROW_HEAD;
}

QWidget* MyClassicArrowHead::getFeatureMenu() {
    QWidget* featureMenu = MyArrowHeadBase::getFeatureMenu();
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();

    // add change shape button
    addChangeShapeStyleButtonToFeatureMenu(featureMenu);

    return featureMenu;
}
