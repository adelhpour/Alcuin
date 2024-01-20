#include "negui_network_element_base.h"
#include "negui_network_element_feature_menu.h"

#include <QGridLayout>

// MyNetworkElementBase

MyNetworkElementBase::MyNetworkElementBase(const QString& name) : MyBase(name) {
    _style = NULL;
    _isActive = false;
    _isSelected = false;
}

MyNetworkElementBase::~MyNetworkElementBase() {
    delete _graphicsItem;
}

void MyNetworkElementBase::setName(const QString& name) {
    _name = name;
}

MyNetworkElementGraphicsItemBase* MyNetworkElementBase::graphicsItem() {
    return _graphicsItem;
}

void MyNetworkElementBase::updateGraphicsItem() {
    graphicsItem()->update(style()->shapeStyles(), calculateZValue());
}

void MyNetworkElementBase::updateFocusedGraphicsItems() {
    graphicsItem()->updateFocusedGraphicsItems();
}

void MyNetworkElementBase::connectGraphicsItem() {
    connect(_graphicsItem, &MyNetworkElementGraphicsItemBase::askForDeleteNetworkElement, this, [this] () { emit askForDeleteNetworkElement(this); });
    connect(_graphicsItem, SIGNAL(askForWhetherNetworkElementIsSelected()), this, SLOT(isSelected()));
    connect(_graphicsItem, SIGNAL(askForEnableFeatureMenuDisplay()), this, SIGNAL(askForEnableFeatureMenuDisplay()));
    connect(_graphicsItem, &MyNetworkElementGraphicsItemBase::askForDisplayFeatureMenu, this, [this] () { emit askForDisplayFeatureMenu(this); });
    connect(_graphicsItem, &MyNetworkElementGraphicsItemBase::askForCopyNetworkElement, this, [this] () { emit askForCopyNetworkElement(this); } );
    connect(_graphicsItem, &MyNetworkElementGraphicsItemBase::askForCutNetworkElement, this, [this] () { emit askForCutNetworkElement(this); } );
    connect(_graphicsItem, &MyNetworkElementGraphicsItemBase::askForCopyNetworkElementStyle, this, [this] () { emit askForCopyNetworkElementStyle(this->style()); } );
    connect(_graphicsItem, &MyNetworkElementGraphicsItemBase::askForPasteNetworkElementStyle, this, [this] () { emit askForPasteNetworkElementStyle(this); } );
    connect(_graphicsItem, SIGNAL(askForWhetherAnyOtherElementsAreSelected()), this, SLOT(areAnyOtherElementsSelected()));
    connect(_graphicsItem, SIGNAL(askForWhetherElementStyleIsCopied()), this, SIGNAL(askForWhetherElementStyleIsCopied()));
    connect(_graphicsItem, SIGNAL(askForCreateChangeStageCommand()), this, SIGNAL(askForCreateChangeStageCommand()));
    connect(_graphicsItem, SIGNAL(askForDisplaySceneContextMenu(const QPointF&)), this, SIGNAL(askForDisplaySceneContextMenu(const QPointF&)));
}

MyNetworkElementStyleBase* MyNetworkElementBase::style() {
    return _style;
}

void MyNetworkElementBase::setStyle(MyNetworkElementStyleBase* style) {
    _style = style;
}

void MyNetworkElementBase::updateStyle(QList<MyShapeStyleBase*> shapeStyles) {
    style()->setShapeStyles(shapeStyles);
}

const QString MyNetworkElementBase::styleCategory() {
    if (!style()->category().isEmpty())
        return style()->category();

    return "N/A";
}

bool MyNetworkElementBase::setActive(const bool& active) {
    return _isActive = active;
}

void MyNetworkElementBase::setSelected(const bool& selected) {
    if (selected != isSelected()) {
        _isSelected = selected;
        graphicsItem()->setFocused(selected);
    }
}

const bool MyNetworkElementBase::isSelected() {
    return _isSelected;
}

const bool MyNetworkElementBase::areAnyOtherElementsSelected() {
    QList<MyNetworkElementBase*> networkElements;
    networkElements.push_back(this);
    return askForWhetherAnyOtherElementsAreSelected(networkElements);
}

void MyNetworkElementBase::enableNormalMode() {
    MySceneModeElementBase::enableNormalMode();
    graphicsItem()->enableNormalMode();
}

void MyNetworkElementBase::enableAddNodeMode() {
    MySceneModeElementBase::enableAddNodeMode();
    graphicsItem()->enableAddNodeMode();
}

void MyNetworkElementBase::enableSelectNodeMode() {
    MySceneModeElementBase::enableSelectNodeMode();
    graphicsItem()->enableSelectNodeMode();
}

void MyNetworkElementBase::enableAddEdgeMode() {
    MySceneModeElementBase::enableAddEdgeMode();
    graphicsItem()->enableAddEdgeMode();
}

void MyNetworkElementBase::enableSelectEdgeMode() {
    MySceneModeElementBase::enableSelectEdgeMode();
    graphicsItem()->enableSelectEdgeMode();
}

QWidget* MyNetworkElementBase::getFeatureMenu() {
    MyFeatureMenuItemFrame* featureMenu = new MyFeatureMenuItemFrame();
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();

    // title
    contentLayout->addWidget(new MyTitleLabel(styleCategory()), contentLayout->rowCount(), 0, 1, 2, Qt::AlignCenter);

    // spacer
    addSpacerItemToFeatureMenu(featureMenu);

    // name
    QString nameTitle = "Name";
    if (!style()->nameTitle().isEmpty())
        nameTitle = style()->nameTitle();
    contentLayout->addWidget(new MyLabel(nameTitle, "Unique " + nameTitle + " of " + styleCategory()), contentLayout->rowCount(), 0, Qt::AlignLeft);
    if (style()->isNameEditable()) {
        QLineEdit* nameLineEdit = new MyRestrictedToNameConventionsLineEdit(name());
        connect(nameLineEdit, &QLineEdit::editingFinished, this, [this, nameLineEdit] () {
            if (!askForCheckWhetherNetworkElementNameIsAlreadyUsed(nameLineEdit->text()))
                setName(nameLineEdit->text());
            nameLineEdit->setText(name());
        } );
        contentLayout->addWidget(nameLineEdit, contentLayout->rowCount() - 1, 1, Qt::AlignRight);
    }
    else
        contentLayout->addWidget(new MyReadOnlyLineEdit(name()), contentLayout->rowCount() - 1, 1, Qt::AlignRight);


    return featureMenu;
}

void MyNetworkElementBase::addSpacerItemToFeatureMenu(QWidget* featureMenu) {
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();
    QLayoutItem* spacerItem = new MySpacerItem(0, 10);
    contentLayout->addItem(spacerItem, contentLayout->rowCount(), 0, 1, 2);
}

void MyNetworkElementBase::addChangeShapeStyleButtonToFeatureMenu(QWidget* featureMenu) {
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();
    QWidget* shapeStylesButtons = style()->shapeStylesButtons();
    if (shapeStylesButtons) {
        connect(shapeStylesButtons, SIGNAL(askForChangeShapeStyle(MyShapeStyleBase*)), featureMenu, SIGNAL(askForChangeShapeStyle(MyShapeStyleBase*)));
        contentLayout->addWidget(shapeStylesButtons, contentLayout->rowCount(), 0, 1, 2, Qt::AlignRight);
    }
}

void MyNetworkElementBase::addAddRemoveShapeStyleButtonsToFeatureMenu(QWidget* featureMenu) {
    QGridLayout* contentLayout = (QGridLayout*)featureMenu->layout();
    QWidget* shapeStylesButtons = style()->shapeStylesButtons();
    connect(shapeStylesButtons, SIGNAL(askForAddShapeStyle(MyShapeStyleBase*)), featureMenu, SIGNAL(askForAddShapeStyle(MyShapeStyleBase*)));
    connect(shapeStylesButtons, SIGNAL(askForRemoveShapeStyle(MyShapeStyleBase*)), featureMenu, SIGNAL(askForRemoveShapeStyle(MyShapeStyleBase*)));
    connect(featureMenu, SIGNAL(askForSetRemovingMenu(QList<MyShapeStyleBase*>)), shapeStylesButtons, SLOT(setRemovingMenu(QList<MyShapeStyleBase*>)));
    contentLayout->addWidget(shapeStylesButtons, contentLayout->rowCount(), 0, 1, 2, Qt::AlignRight);
}

QWidget* MyNetworkElementBase::createFeatureMenu() {
    MyFeatureMenuBase* featureMenu =  new MySingleNetworkElementFeatureMenu(getFeatureMenu(), askForIconsDirectoryPath());
    featureMenu->setObjectName(name());
    ((MySingleNetworkElementFeatureMenu*)featureMenu)->setShapeStyles(style()->shapeStyles());
    connect((MySingleNetworkElementFeatureMenu*)featureMenu, &MySingleNetworkElementFeatureMenu::isUpdated, this, [this] (QList<MyShapeStyleBase*> shapeStyles) {
        updateStyle(shapeStyles);
        updateGraphicsItem();
        updateFocusedGraphicsItems();
        emit askForCreateChangeStageCommand(); } );
    QWidget* currentlyBeingDisplayedFeatureMenu = askForCurrentlyBeingDisplayedFeatureMenu();
    if (currentlyBeingDisplayedFeatureMenu && ((MyFeatureMenuBase*)currentlyBeingDisplayedFeatureMenu)->type() == MyFeatureMenuBase::ELEMENT_FEATURE_MENU)
        ((MySingleNetworkElementFeatureMenu*)featureMenu)->setBeingModifiedShapeStyle(((MySingleNetworkElementFeatureMenu*)currentlyBeingDisplayedFeatureMenu)->beingModifiedShapeStyle());

    return featureMenu;

}

QString getElementUniqueName(QList<MyNetworkElementBase*> elements, const QString& defaultNameSection) {
    QString name;
    qreal k = 0;
    bool isSimilarNameFound = true;
    while(isSimilarNameFound) {
        name = defaultNameSection + "_" + QString::number(k);
        isSimilarNameFound = false;
        for (MyNetworkElementBase *element : qAsConst(elements)) {
            if (element->name() == name) {
                isSimilarNameFound = true;
                break;
            }
        }
        ++k;
    }

    return name;
}

MyNetworkElementBase* findElement(QList<MyNetworkElementBase*> elements, const QString& name) {
    for (MyNetworkElementBase *element : qAsConst(elements)) {
        if (element->name() == name)
            return element;
    }

    return NULL;
}

MyNetworkElementBase* findSourceNode(QList<MyNetworkElementBase*> nodes, const QJsonObject &json) {
    if (json.contains("source") && json["source"].isObject() && json["source"].toObject().contains("node") && json["source"]["node"].isString())
        return findElement(nodes, json["source"]["node"].toString());

    return NULL;
}

MyNetworkElementBase* findTargetNode(QList<MyNetworkElementBase*> nodes, const QJsonObject &json) {
    if (json.contains("target") && json["target"].isObject() && json["target"].toObject().contains("node") && json["target"]["node"].isString())
        return findElement(nodes, json["target"]["node"].toString());

    return NULL;
}

const bool whetherNetworkElementExistsInTheListOfNetworkElements(MyNetworkElementBase* networkElement, QList<MyNetworkElementBase*> networkElements) {
    for (MyNetworkElementBase* element : networkElements) {
        if (networkElement == element)
            return true;
    }

    return false;
}

MyNetworkElementBase* getNetworkElement(QList<MyNetworkElementBase*> networkElements, const QString& name) {
    for (MyNetworkElementBase* networkElement : networkElements) {
        if (networkElement->name() == name)
            return networkElement;
    }
    return  NULL;
}

QList<MyNetworkElementBase*> getNetworkElementsOfType(QList<MyNetworkElementBase*> networkElements, const QString& type) {
    QList<MyNetworkElementBase*> networkElementsOfType;
    for (MyNetworkElementBase* networkElement : networkElements) {
        if (networkElement->type() == type)
            networkElementsOfType.push_back(networkElement);
    }

    return networkElementsOfType;
}

QList<MyNetworkElementBase*> getNetworkElementsOfCategory(QList<MyNetworkElementBase*> networkElements, const QString& category) {
    QList<MyNetworkElementBase*> networkElementsOfCategory;
    for (MyNetworkElementBase* networkElement : networkElements) {
        if (networkElement->style()->category() == category)
            networkElementsOfCategory.push_back(networkElement);
    }

    return networkElementsOfCategory;
}

QList<MyNetworkElementBase*> getNetworkElementsOfSubCategory(QList<MyNetworkElementBase*> networkElements, const QString& subCategory) {
    QList<MyNetworkElementBase*> networkElementsOfSubCategory;
    for (MyNetworkElementBase* networkElement : networkElements) {
        if (networkElement->style()->subCategory() == subCategory)
            networkElementsOfSubCategory.push_back(networkElement);
    }

    return networkElementsOfSubCategory;
}
