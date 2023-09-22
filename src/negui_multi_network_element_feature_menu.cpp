#include "negui_multi_network_element_feature_menu.h"

#include <QGridLayout>

// MyMultiNetworkElementFeatureMenu

MyMultiNetworkElementFeatureMenu::MyMultiNetworkElementFeatureMenu(const QString& iconsDirectoryPath, QWidget *parent) : MyFeatureMenuBase(iconsDirectoryPath, parent) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    contentLayout->addWidget(new MyTitleLabel("Menu"), contentLayout->rowCount(), 0, 1, 2, Qt::AlignHCenter | Qt::AlignTop);
    contentLayout->setAlignment(Qt::AlignTop);
    setFixedWidth(340);
}

MyMultiNetworkElementFeatureMenu::FEATURE_MENU_TYPE MyMultiNetworkElementFeatureMenu::type() {
    return MULTI_NETWORK_ELEMENT_FEATURE_MENU;
}
