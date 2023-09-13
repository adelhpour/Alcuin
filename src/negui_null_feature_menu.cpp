#include "negui_null_feature_menu.h"

#include <QGridLayout>

// MyNullFeatureMenu

MyNullFeatureMenu::MyNullFeatureMenu(const QString& iconsDirectoryPath, QWidget *parent) : MyFeatureMenuBase(iconsDirectoryPath, parent) {
    QGridLayout* contentLayout = (QGridLayout*)layout();
    contentLayout->addWidget(new MyLabel("There are no elements selected"), contentLayout->rowCount(), 0, 1, 2, Qt::AlignLeft);
}

MyFeatureMenuBase::FEATURE_MENU_TYPE MyNullFeatureMenu::type() {
    return NULL_FEATURE_MENU;
}