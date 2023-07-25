#ifndef __NEGUI_DECORATE_MENU_BUTTONS_H
#define __NEGUI_DECORATE_MENU_BUTTONS_H

#include <QToolButton>
#include <QWidgetAction>

void decorateImportButton(QToolButton* button, const QString& iconsDirectory);

void decorateExportButton(QToolButton* button, const QString& iconsDirectory);

void decorateSaveButton(QToolButton* button, const QString& iconsDirectory);

void decorateAutoLayoutButton(QToolButton* button, const QString& iconsDirectory);

void decorateUndoActionButton(QToolButton* button, const QString& iconsDirectory);

void decorateRedoActionButton(QToolButton* button, const QString& iconsDirectory);

void decorateResetSceneButton(QToolButton* button, const QString& iconsDirectory);

void decorateNormalModeButton(QToolButton* button, const QString& iconsDirectory);

void decorateZoomInButton(QToolButton* button, const QString& iconsDirectory);

void decorateZoomOutButton(QToolButton* button, const QString& iconsDirectory);

QToolButton* decorateAddModeButton(QList<QToolButton*> buttons, const QString& iconsDirectory);

void decorateAddModeButton(QToolButton* button, const QString& iconsDirectory);

QMenu* createAddButtonMenu(QList<QToolButton*> buttons);

QWidgetAction* createButtonWidgetAction(QToolButton* button, QWidget* parent);

void setIcon(QToolButton* button, const QString& iconPath);

#endif
