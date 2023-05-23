#ifndef __NEGUI_DECORATE_MENU_BUTTONS_H
#define __NEGUI_DECORATE_MENU_BUTTONS_H

#include <QToolButton>
#include <QWidgetAction>

void decorateImportButton(QToolButton* button);

void decorateExportButton(QToolButton* button);

void decorateAutoLayoutButton(QToolButton* button);

void decorateUndoActionButton(QToolButton* button);

void decorateRedoActionButton(QToolButton* button);

void decorateResetSceneButton(QToolButton* button);

void decorateNormalModeButton(QToolButton* button);

void decorateSelectModeButton(QToolButton* button);

void decorateRemoveModeButton(QToolButton* button);

void decorateZoomInButton(QToolButton* button);

void decorateZoomOutButton(QToolButton* button);

QToolButton* decorateAddModeButton(QList<QToolButton*> buttons);

void decorateAddModeButton(QToolButton* button);

QMenu* createAddButtonMenu(QList<QToolButton*> buttons);

QWidgetAction* createButtonWidgetAction(QToolButton* button, QWidget* parent);

void setIcon(QToolButton* button, const QString& iconPath);

#endif
