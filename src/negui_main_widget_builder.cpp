#include "negui_main_widget_builder.h"
#include "negui_main_widget.h"

QWidget* createNetworkEditorWidget(QWidget* parent) {
    return new MyMainWidget(parent);
}
