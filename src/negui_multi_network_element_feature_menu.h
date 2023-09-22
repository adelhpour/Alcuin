#ifndef __NEGUI_MULTI_ELEMENT_FEATURE_MENU_H
#define __NEGUI_MULTI_ELEMENT_FEATURE_MENU_H

#include "negui_feature_menu_base.h"

class MyMultiNetworkElementFeatureMenu : public MyFeatureMenuBase {
    Q_OBJECT

public:

    MyMultiNetworkElementFeatureMenu(const QString& iconsDirectoryPath, QWidget *parent = nullptr);

    FEATURE_MENU_TYPE type() override;
};

#endif
