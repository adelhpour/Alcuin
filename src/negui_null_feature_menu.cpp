#include "negui_null_feature_menu.h"

#include <QGridLayout>

// MyNullFeatureMenu

MyNullFeatureMenu::MyNullFeatureMenu(const QString& iconsDirectoryPath, QWidget *parent) : MyFeatureMenuBase(iconsDirectoryPath, parent) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    contentLayout->addWidget(new MyTitleLabel("No Element"), contentLayout->rowCount(), 0, 1, 2, Qt::AlignHCenter | Qt::AlignTop);
    contentLayout->addWidget(new MyNoSelectedElementTitle(), contentLayout->rowCount(), 0, 1, 2, Qt::AlignHCenter | Qt::AlignTop);
    contentLayout->setAlignment(Qt::AlignTop);
    setFixedWidth(340);
}

MyFeatureMenuBase::FEATURE_MENU_TYPE MyNullFeatureMenu::type() {
    return NULL_FEATURE_MENU;
}