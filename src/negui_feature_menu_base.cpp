#include "negui_feature_menu_base.h"
#include "negui_menu_button_builder.h"

#include <QGridLayout>

// MyFeatureMenuBase

MyFeatureMenuBase::MyFeatureMenuBase(const QString& iconsDirectoryPath, QWidget *parent) : MyFrame(parent) {
    QGridLayout* contentLayout = new QGridLayout(this);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

    QAbstractButton* closeFeatureMenuButton = createCloseFeatureMenuButton(this, iconsDirectoryPath);
    contentLayout->addWidget(closeFeatureMenuButton, contentLayout->rowCount(), 0, 1, 2, Qt::AlignLeft);
}