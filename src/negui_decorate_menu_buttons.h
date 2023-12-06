#ifndef __NEGUI_DECORATE_MENU_BUTTONS_H
#define __NEGUI_DECORATE_MENU_BUTTONS_H

#include <QAbstractButton>
#include <QPushButton>
#include <QWidgetAction>

void decorateImportButton(QAbstractButton* button, const QString& iconsDirectory);

void decorateDataExportButton(QAbstractButton* button, const QString& iconsDirectory);

void decoratePrintExportButton(QAbstractButton* button, const QString& iconsDirectory);

void decorateSaveButton(QAbstractButton* button, const QString& iconsDirectory);

void decorateDefaultButton(QAbstractButton* button, const QString& iconsDirectory);

void decorateUndoActionButton(QAbstractButton* button, const QString& iconsDirectory);

void decorateRedoActionButton(QAbstractButton* button, const QString& iconsDirectory);

void decorateNewNetworkCanvasButton(QAbstractButton* button, const QString& iconsDirectory);

void decorateNormalModeButton(QAbstractButton* button, const QString& iconsDirectory);

void decorateZoomInButton(QAbstractButton* button, const QString& iconsDirectory);

void decorateZoomOutButton(QAbstractButton* button, const QString& iconsDirectory);

QWidgetAction* createButtonWidgetAction(QAbstractButton* button, QWidget* parent);

void decorateExtraElementsMenuButton(QAbstractButton* button, const QString& iconsDirectory);

void decorateCloseFeatureMenuButton(QAbstractButton* button, const QString& iconsDirectory);

void setIcon(QAbstractButton* button, const QString& iconPath);

#endif
