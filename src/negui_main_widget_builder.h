#ifndef __NEGUI_MAIN_WIDGET_BUILDER_H
#define __NEGUI_MAIN_WIDGET_BUILDER_H

#if defined MAKE_NEGUI_LIB
#define NEGUI_LIB_EXPORT Q_DECL_EXPORT
#else
#define NEGUI_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QWidget>

NEGUI_LIB_EXPORT QWidget* createNetworkEditorWidget(QWidget* parent = NULL);

#endif
