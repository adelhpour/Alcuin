#include "negui_feature_menu_base.h"
#include "negui_decorate_menu_buttons.h"

#include <QGridLayout>

// MyFeatureMenuBase

MyFeatureMenuBase::MyFeatureMenuBase(const QString& iconsDirectoryPath, QWidget *parent) : MyFrame(parent) {
    QGridLayout* contentLayout = new QGridLayout(this);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

    QPushButton* closeFeatureMenuButton = new QPushButton();
    decorateCloseFeatureMenuButton(closeFeatureMenuButton, iconsDirectoryPath);
    connect(closeFeatureMenuButton, SIGNAL(clicked()), this, SIGNAL(askForRemoveFeatureMenu()));
    contentLayout->addWidget(closeFeatureMenuButton, contentLayout->rowCount(), 0, 1, 2, Qt::AlignLeft);
}