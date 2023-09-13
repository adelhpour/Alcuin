#ifndef __NEGUI_NULL_FEATURE_MENU_H
#define __NEGUI_NULL_FEATURE_MENU_H

#include "negui_feature_menu_base.h"

class MyNullFeatureMenu : public MyFeatureMenuBase {
    Q_OBJECT

public:

    MyNullFeatureMenu(const QString& iconsDirectoryPath, QWidget *parent = nullptr);

    FEATURE_MENU_TYPE type() override;
};

#endif
