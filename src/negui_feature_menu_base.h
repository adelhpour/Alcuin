#ifndef __NEGUI_FEATURE_MENU_BASE_H
#define __NEGUI_FEATURE_MENU_BASE_H

#include "negui_customized_feature_menu_widgets.h"

class MyFeatureMenuBase : public MyFrame {
    Q_OBJECT

public:

    typedef enum {
        NULL_FEATURE_MENU,
        ELEMENT_FEATURE_MENU,
    } FEATURE_MENU_TYPE;

    MyFeatureMenuBase(const QString& iconsDirectoryPath, QWidget *parent = nullptr);

    virtual FEATURE_MENU_TYPE type() = 0;

signals:

    void askForRemoveFeatureMenu();
};

#endif
