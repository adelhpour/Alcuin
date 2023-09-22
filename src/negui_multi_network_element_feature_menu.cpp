#include "negui_multi_network_element_feature_menu.h"

#include <QGridLayout>

// MyMultiNetworkElementFeatureMenu

MyMultiNetworkElementFeatureMenu::MyMultiNetworkElementFeatureMenu(QList<MyNetworkElementBase*> networkElements, const QString& iconsDirectoryPath, QWidget *parent) : MyFeatureMenuBase(iconsDirectoryPath, parent) {
    _networkElements = networkElements;
    QGridLayout* contentLayout = (QGridLayout*)layout();
    contentLayout->addWidget(new MyTitleLabel("Menu"), contentLayout->rowCount(), 0, 1, 2, Qt::AlignHCenter | Qt::AlignTop);
    MyParameterBase* parameter = NULL;
    MyParameterBase* representativeBorderWidthParameter = createRepresentativeBorderWidthParameter();
    if (representativeBorderWidthParameter) {
        contentLayout->addWidget(new MyLabel((representativeBorderWidthParameter->name()), representativeBorderWidthParameter->hint()), contentLayout->rowCount(), 0, Qt::AlignLeft);
        contentLayout->addWidget(representativeBorderWidthParameter->inputWidget(), contentLayout->rowCount() - 1, 1, Qt::AlignRight);
    }
    MyParameterBase* representativeBorderColorParameter = createRepresentativeBorderColorParameter();
    if (representativeBorderColorParameter) {
        contentLayout->addWidget(new MyLabel((representativeBorderColorParameter->name()), representativeBorderColorParameter->hint()), contentLayout->rowCount(), 0, Qt::AlignLeft);
        contentLayout->addWidget(representativeBorderColorParameter->inputWidget(), contentLayout->rowCount() - 1, 1, Qt::AlignRight);
    }
    MyParameterBase* representativeFillColorParameter = createRepresentativeFillColorParameter();
    if (representativeFillColorParameter) {
        contentLayout->addWidget(new MyLabel((representativeFillColorParameter->name()), representativeFillColorParameter->hint()), contentLayout->rowCount(), 0, Qt::AlignLeft);
        contentLayout->addWidget(representativeFillColorParameter->inputWidget(), contentLayout->rowCount() - 1, 1, Qt::AlignRight);
    }
    contentLayout->setAlignment(Qt::AlignTop);
    setFixedWidth(340);
}

MyMultiNetworkElementFeatureMenu::FEATURE_MENU_TYPE MyMultiNetworkElementFeatureMenu::type() {
    return MULTI_NETWORK_ELEMENT_FEATURE_MENU;
}

MyParameterBase* MyMultiNetworkElementFeatureMenu::createRepresentativeBorderWidthParameter() {
    MyParameterBase* representativeBorderWidthParameter = NULL;
    if (getNetworkElementParameters("border-width").size()) {
        representativeBorderWidthParameter = new MyBorderWidthParameter();
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

MyParameterBase* MyMultiNetworkElementFeatureMenu::createRepresentativeBorderColorParameter() {
    MyParameterBase* representativeBorderColorParameter = NULL;
    if (getNetworkElementParameters("border-color").size()) {
        representativeBorderColorParameter = new MyBorderColorParameter();
        connect(representativeBorderColorParameter, &MyParameterBase::isUpdated, this, [this, representativeBorderColorParameter] () {
            representativeBorderColorParameter->setDefaultValue();
            updateBorderColorParameters(((MyBorderColorParameter*)representativeBorderColorParameter)->defaultValue());
            updateNetworkElements();
        });
    }

    return representativeBorderColorParameter;
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
        connect(representativeFillColorParameter, &MyParameterBase::isUpdated, this, [this, representativeFillColorParameter] () {
            representativeFillColorParameter->setDefaultValue();
            updateFillColorParameters(((MyFillColorParameter*)representativeFillColorParameter)->defaultValue());
            updateNetworkElements();
        });
    }

    return representativeFillColorParameter;
}

void MyMultiNetworkElementFeatureMenu::updateFillColorParameters(const QString& fillColor) {
    QList<MyParameterBase*> fillColorParameters = getNetworkElementParameters("fill-color");
    for (MyParameterBase* fillColorParameter : fillColorParameters)
        ((MyFillColorParameter*)fillColorParameter)->setDefaultValue(fillColor);
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