#ifndef __NEGUI_C_API_H
#define __NEGUI_C_API_H

#include "negui_mainwidget.h"

extern "C" {

NEGUI_LIB_EXPORT QWidget* createNetworkEditorWidget(QWidget* parent = NULL);

NEGUI_LIB_EXPORT void deleteNetworkEditorWidget(QWidget* networkEditorWidget);

NEGUI_LIB_EXPORT void setGraphInfoNetworkEditorWidget(QWidget* networkEditorWidget, const char* graphInfo);

NEGUI_LIB_EXPORT const char* getGraphInfoNetworkEditorWidget(QWidget* networkEditorWidget);

}

#endif
