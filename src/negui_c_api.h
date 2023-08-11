#ifndef __NEGUI_C_API_H
#define __NEGUI_C_API_H

#if defined MAKE_NEGUI_LIB
#define NEGUI_LIB_EXPORT Q_DECL_EXPORT
#else
#define NEGUI_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QWidget>

extern "C" {

NEGUI_LIB_EXPORT QWidget* createNetworkEditorWidget(QWidget* parent = NULL);

NEGUI_LIB_EXPORT void deleteNetworkEditorWidget(QWidget* networkEditorWidget);

NEGUI_LIB_EXPORT void setGraphInfoNetworkEditorWidget(QWidget* networkEditorWidget, const char* graphInfo);

NEGUI_LIB_EXPORT const char* getGraphInfoNetworkEditorWidget(QWidget* networkEditorWidget);

NEGUI_LIB_EXPORT const char* getName();

NEGUI_LIB_EXPORT const char* getVersionNumber();

}

#endif
