#ifndef __NEGUI_CUSTOMIZED_MODE_MENU_WIDGETS_H
#define __NEGUI_CUSTOMIZED_MODE_MENU_WIDGETS_H

#include "negui_customized_common_widgets.h"
#include "negui_scene_mode_element_base.h"

class MyModeMenuToolButton : public MyToolButton {
    Q_OBJECT

public:

    MyModeMenuToolButton(QWidget* parent = nullptr);

    void setActive(const bool& active);

    void setStyleToActiveForm();

    void setStyleToInactiveForm();
};

class MyModeMenuModeButton : public MyModeMenuToolButton, public MySceneModeElementBase {
    Q_OBJECT

public:

    MyModeMenuModeButton(const QString& mode = "", const QString& alternativeSimilarMode = "", QWidget* parent = nullptr);
};

#endif
