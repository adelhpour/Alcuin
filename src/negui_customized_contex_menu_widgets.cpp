#include "negui_customized_context_menu_widgets.h"

// MyCheckableAction

MyCheckableAction::MyCheckableAction(const QString &text, QObject *parent) : QAction(text, parent) {
    setCheckable(true);
}