#include "negui_multi_network_element_feature_menu.h"

#include <QGridLayout>

// MyMultiNetworkElementFeatureMenu

MyMultiNetworkElementFeatureMenu::MyMultiNetworkElementFeatureMenu(QList<MyNetworkElementBase*> networkElements, const QString& iconsDirectoryPath, QWidget *parent) : MyFeatureMenuBase(iconsDirectoryPath, parent) {
    _networkElements = networkElements;
    QGridLayout* contentLayout = (QGridLayout*)layout();
    contentLayout->addWidget(new MyTitleLabel("Selected Elements"), contentLayout->rowCount(), 0, 1, 2, Qt::AlignHCenter | Qt::AlignTop);
    QLayoutItem* spacerItem = new MySpacerItem(0, 30);
    contentLayout->addItem(spacerItem, contentLayout->rowCount(), 0, 1, 2);
    addItems();
    contentLayout->setAlignment(Qt::AlignTop);
    setFixedWidth(340);
}

MyMultiNetworkElementFeatureMenu::FEATURE_MENU_TYPE MyMultiNetworkElementFeatureMenu::type() {
    return MULTI_NETWORK_ELEMENT_FEATURE_MENU;
}

void MyMultiNetworkElementFeatureMenu::addItems() {
    // parameters
    MyParameterBase* parameter = NULL;
    addParameter(createRepresentativeBorderWidthParameter());
    addParameter(createRepresentativeBorderColorParameter());
    addParameter(createRepresentativeFillColorParameter());
    addParameter(createRepresentativeTextColorParameter());

    // shape style buttons
    addShapeStyleButtons();
}

void MyMultiNetworkElementFeatureMenu::addParameter(MyParameterBase* parameter) {
    if (parameter) {
        QGridLayout* contentLayout = (QGridLayout*)layout();
        contentLayout->addWidget(new MyLabel((parameter->name()), parameter->hint()), contentLayout->rowCount(), 0, Qt::AlignLeft);
        contentLayout->addWidget(parameter->inputWidget(), contentLayout->rowCount() - 1, 1, Qt::AlignRight);
    }
}

MyParameterBase* MyMultiNetworkElementFeatureMenu::createRepresentativeBorderWidthParameter() {
    MyParameterBase* representativeBorderWidthParameter = NULL;
    if (getNetworkElementParameters("border-width").size()) {
        representativeBorderWidthParameter = new MyBorderWidthParameter();
        ((MyBorderWidthParameter*)representativeBorderWidthParameter)->setDefaultValue(getRepresentativeBorderWidthParameterDefaultValue());
        connect(representativeBorderWidthParameter, &MyParameterBase::isUpdated, this, [this, representativeBorderWidthParameter] () {
            representativeBorderWidthParameter->setDefaultValue();
            updateBorderWidthParameters(((MyBorderWidthParameter*)representativeBorderWidthParameter)->defaultValue());
            updateNetworkElements();
        });
    }

    return representativeBorderWidthParameter;
}

void MyMultiNetworkElementFeatureMenu::updateBorderWidthParameters(const qint32& borderWidth) {
    QList<MyParameterBase*> borderWidthParameters = getNetworkElementParameters("border-width");
    for (MyParameterBase* borderWidthParameter : borderWidthParameters)
        ((MyBorderWidthParameter*)borderWidthParameter)->setDefaultValue(borderWidth);
}

const qint32 MyMultiNetworkElementFeatureMenu::getRepresentativeBorderWidthParameterDefaultValue() {
    qint32 borderWidthParameterDefaultValue;
    QList<MyParameterBase*> borderWidthParameters = getNetworkElementParameters("border-width");
    for (MyParameterBase* borderWidthParameter : borderWidthParameters) {
        if (((MyBorderWidthParameter*)borderWidthParameter)->defaultValue() != borderWidthParameterDefaultValue)
            borderWidthParameterDefaultValue = ((MyBorderWidthParameter*)borderWidthParameter)->defaultValue();
    }

    return borderWidthParameterDefaultValue;
}

const qint32 MyMultiNetworkElementFeatureMenu::getCommonBorderWidthParameterValueDefaultValue() {
    qint32 commonBorderWidthParameterDefaultValue = 0;
    QList<MyParameterBase*> borderWidthParameters = getNetworkElementParameters("border-width");
    for (MyParameterBase* borderWidthParameter : borderWidthParameters) {
        if (commonBorderWidthParameterDefaultValue == 0)
            commonBorderWidthParameterDefaultValue = ((MyBorderWidthParameter*)borderWidthParameter)->defaultValue();
        else if (((MyBorderWidthParameter*)borderWidthParameter)->defaultValue() != commonBorderWidthParameterDefaultValue)
            return 0;
    }

    return commonBorderWidthParameterDefaultValue;
}

MyParameterBase* MyMultiNetworkElementFeatureMenu::createRepresentativeBorderColorParameter() {
    MyParameterBase* representativeBorderColorParameter = NULL;
    if (getNetworkElementParameters("border-color").size()) {
        representativeBorderColorParameter = new MyBorderColorParameter();
        ((MyBorderColorParameter*)representativeBorderColorParameter)->setDefaultValue(getRepresentativeBorderColorParameterDefaultValue());
        connect(representativeBorderColorParameter, &MyParameterBase::isUpdated, this, [this, representativeBorderColorParameter] () {
            representativeBorderColorParameter->setDefaultValue();
            updateBorderColorParameters(((MyBorderColorParameter*)representativeBorderColorParameter)->defaultValue());
            updateNetworkElements();
        });
    }

    return representativeBorderColorParameter;
}

const QString MyMultiNetworkElementFeatureMenu::getRepresentativeBorderColorParameterDefaultValue() {
    QString borderColorParameterDefaultValue;
    QList<MyParameterBase*> borderColorParameters = getNetworkElementParameters("border-color");
    for (MyParameterBase* borderColorParameter : borderColorParameters) {
        if (((MyBorderColorParameter*)borderColorParameter)->defaultValue() != borderColorParameterDefaultValue)
            borderColorParameterDefaultValue = ((MyBorderColorParameter*)borderColorParameter)->defaultValue();
    }

    return borderColorParameterDefaultValue;
}

const QString MyMultiNetworkElementFeatureMenu::getCommonBorderColorParameterValueDefaultValue() {
    QString commonBorderColorParameterDefaultValue;
    QList<MyParameterBase*> borderColorParameters = getNetworkElementParameters("border-color");
    for (MyParameterBase* borderColorParameter : borderColorParameters) {
        if (commonBorderColorParameterDefaultValue.isEmpty())
            commonBorderColorParameterDefaultValue = ((MyBorderColorParameter*)borderColorParameter)->defaultValue();
        else if (((MyBorderColorParameter*)borderColorParameter)->defaultValue() != commonBorderColorParameterDefaultValue)
            return "";
    }

    return commonBorderColorParameterDefaultValue;
}

void MyMultiNetworkElementFeatureMenu::updateBorderColorParameters(const QString& borderColor) {
    QList<MyParameterBase*> borderColorParameters = getNetworkElementParameters("border-color");
    for (MyParameterBase* borderColorParameter : borderColorParameters)
        ((MyBorderColorParameter*)borderColorParameter)->setDefaultValue(borderColor);
}

MyParameterBase* MyMultiNetworkElementFeatureMenu::createRepresentativeFillColorParameter() {
    MyParameterBase* representativeFillColorParameter = NULL;
    if (getNetworkElementParameters("fill-color").size()) {
        representativeFillColorParameter = new MyFillColorParameter();
        ((MyFillColorParameter*)representativeFillColorParameter)->setDefaultValue(getRepresentativeFillColorParameterDefaultValue());
        connect(representativeFillColorParameter, &MyParameterBase::isUpdated, this, [this, representativeFillColorParameter] () {
            representativeFillColorParameter->setDefaultValue();
            updateFillColorParameters(((MyFillColorParameter*)representativeFillColorParameter)->defaultValue());
            updateNetworkElements();
        });
    }

    return representativeFillColorParameter;
}

const QString MyMultiNetworkElementFeatureMenu::getRepresentativeFillColorParameterDefaultValue() {
    QString fillColorParameterDefaultValue;
    QList<MyParameterBase*> fillColorParameters = getNetworkElementParameters("fill-color");
    for (MyParameterBase* fillColorParameter : fillColorParameters) {
        if (((MyFillColorParameter*)fillColorParameter)->defaultValue() != fillColorParameterDefaultValue)
            fillColorParameterDefaultValue = ((MyFillColorParameter*)fillColorParameter)->defaultValue();
    }

    return fillColorParameterDefaultValue;
}

const QString MyMultiNetworkElementFeatureMenu::getCommonFillColorParameterValueDefaultValue() {
    QString commonFillColorParameterValueDefaultValue;
    QList<MyParameterBase*> fillColorParameters = getNetworkElementParameters("fill-color");
    for (MyParameterBase* fillColorParameter : fillColorParameters) {
        if (commonFillColorParameterValueDefaultValue.isEmpty())
            commonFillColorParameterValueDefaultValue = ((MyFillColorParameter*)fillColorParameter)->defaultValue();
        else if (((MyFillColorParameter*)fillColorParameter)->defaultValue() != commonFillColorParameterValueDefaultValue)
            return "";
    }

    return commonFillColorParameterValueDefaultValue;
}

void MyMultiNetworkElementFeatureMenu::updateFillColorParameters(const QString& fillColor) {
    QList<MyParameterBase*> fillColorParameters = getNetworkElementParameters("fill-color");
    for (MyParameterBase* fillColorParameter : fillColorParameters)
        ((MyFillColorParameter*)fillColorParameter)->setDefaultValue(fillColor);
}

MyParameterBase* MyMultiNetworkElementFeatureMenu::createRepresentativeTextColorParameter() {
    MyParameterBase* representativeTextColorParameter = NULL;
    if (getNetworkElementParameters("text-color").size()) {
        representativeTextColorParameter = new MyTextColorParameter();
        ((MyTextColorParameter*)representativeTextColorParameter)->setDefaultValue(getRepresentativeTextColorParameterDefaultValue());
        connect(representativeTextColorParameter, &MyParameterBase::isUpdated, this, [this, representativeTextColorParameter] () {
            representativeTextColorParameter->setDefaultValue();
            updateTextColorParameters(((MyTextColorParameter*)representativeTextColorParameter)->defaultValue());
            updateNetworkElements();
        });
    }

    return representativeTextColorParameter;
}

void MyMultiNetworkElementFeatureMenu::updateTextColorParameters(const QString& textColor) {
    QList<MyParameterBase*> textColorParameters = getNetworkElementParameters("text-color");
    for (MyParameterBase* textColorParameter : textColorParameters)
        ((MyTextColorParameter*)textColorParameter)->setDefaultValue(textColor);
}

const QString MyMultiNetworkElementFeatureMenu::getRepresentativeTextColorParameterDefaultValue() {
    QString textColorParameterDefaultValue;
    QList<MyParameterBase*> textColorParameters = getNetworkElementParameters("text-color");
    for (MyParameterBase* textColorParameter : textColorParameters) {
        if (((MyTextColorParameter*)textColorParameter)->defaultValue() != textColorParameterDefaultValue)
            textColorParameterDefaultValue = ((MyTextColorParameter*)textColorParameter)->defaultValue();
    }

    return textColorParameterDefaultValue;
}

const QString MyMultiNetworkElementFeatureMenu::getCommonTextColorParameterValueDefaultValue() {
    QString commonTextColorParameterDefaultValue = 0;
    QList<MyParameterBase*> textColorParameters = getNetworkElementParameters("color");
    for (MyParameterBase* textColorParameter : textColorParameters) {
        if (commonTextColorParameterDefaultValue == 0)
            commonTextColorParameterDefaultValue = ((MyTextColorParameter*)textColorParameter)->defaultValue();
        else if (((MyTextColorParameter*)textColorParameter)->defaultValue() != commonTextColorParameterDefaultValue)
            return 0;
    }

    return commonTextColorParameterDefaultValue;
}

void MyMultiNetworkElementFeatureMenu::addShapeStyleButtons() {
    QList<QWidget*> listOfShapeStyleButtons;
    QWidget* shapeStyleButtons = NULL;
    for (MyNetworkElementBase* networkElement : _networkElements) {
        shapeStyleButtons = networkElement->style()->shapeStylesButtons();
        if (shapeStyleButtons)
            listOfShapeStyleButtons.push_back(shapeStyleButtons);
    }

    if (listOfShapeStyleButtons.size()) {
        shapeStyleButtons = listOfShapeStyleButtons.at(0);
        QGridLayout* contentLayout = (QGridLayout*)layout();
        connect(shapeStyleButtons, SIGNAL(askForChangeShapeStyle(MyShapeStyleBase*)), this, SLOT(updateShapeStyles(MyShapeStyleBase*)));
        contentLayout->addWidget(shapeStyleButtons, contentLayout->rowCount(), 0, 1, 2, Qt::AlignRight);
    }
}

void MyMultiNetworkElementFeatureMenu::updateShapeStyles(MyShapeStyleBase* shapeStyle) {
    if (shapeStyle) {
        qint32 borderWidthDefaultValue = getCommonBorderWidthParameterValueDefaultValue();
        QString borderColorDefaultValue = getCommonBorderColorParameterValueDefaultValue();
        QString fillColorDefaultValue = getCommonFillColorParameterValueDefaultValue();
        QString textColorDefaultValue = getCommonTextColorParameterValueDefaultValue();

        for (MyNetworkElementBase* networkElement : _networkElements)
            networkElement->style()->replaceShapeStyle(shapeStyle);

        if (borderWidthDefaultValue != 0)
            updateBorderWidthParameters(borderWidthDefaultValue);
        if (!borderColorDefaultValue.isEmpty())
            updateBorderColorParameters(borderColorDefaultValue);
        if (!fillColorDefaultValue.isEmpty())
            updateFillColorParameters(fillColorDefaultValue);
        if (!textColorDefaultValue.isEmpty())
            updateTextColorParameters(fillColorDefaultValue);

        updateNetworkElements();
    }
}

void MyMultiNetworkElementFeatureMenu::updateNetworkElements() {
    for (MyNetworkElementBase* networkElement : _networkElements) {
        networkElement->updateStyle(networkElement->style()->shapeStyles());
        networkElement->updateGraphicsItem();
        networkElement->updateFocusedGraphicsItems();
    }
    emit askForCreateChangeStageCommand();
}

QList<MyParameterBase*> MyMultiNetworkElementFeatureMenu::getNetworkElementParameters(const QString& parameterName) {
    QList<MyParameterBase*> parameters;
    MyParameterBase* parameter = NULL;
    for (MyNetworkElementBase* networkElement : _networkElements) {
        for (MyShapeStyleBase* shapeStyle : networkElement->style()->shapeStyles()) {
            parameter = shapeStyle->findParameter(parameterName);
            if (parameter)
                parameters.push_back(parameter);
        }
    }

    return parameters;
}