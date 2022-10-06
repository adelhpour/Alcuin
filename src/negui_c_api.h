#ifndef __NEGUI_C_API_H
#define __NEGUI_C_API_H

#include "negui_mainwidget.h"

extern "C" {

typedef struct NEGUI_LIB_EXPORT QWidget QWidget;

typedef struct NEGUI_LIB_EXPORT MyNetworkEditorWidget MyNetworkEditorWidget;

NEGUI_LIB_EXPORT MyNetworkEditorWidget* MyNetworkEditorWidget_new(QWidget* parent = NULL);

NEGUI_LIB_EXPORT void MyNetworkEditorWidget_delete(MyNetworkEditorWidget* myNetworkEditorWidget);

NEGUI_LIB_EXPORT void MyNetworkEditorWidget_setGraphInfo(MyNetworkEditorWidget* myNetworkEditorWidget, const char* graphInfo);

NEGUI_LIB_EXPORT const char* MyNetworkEditorWidget_graphInfo(MyNetworkEditorWidget* myNetworkEditorWidget);
}

#endif
