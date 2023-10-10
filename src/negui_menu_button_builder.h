#ifndef __NEGUI_MENU_BUTTON_BUILDER_H
#define __NEGUI_MENU_BUTTON_BUILDER_H

#include "negui_plugin_item_base.h"

#include <QAbstractButton>
#include <QMenu>

QList<QAbstractButton*> createToolbarMenuButtons(QObject* interactor, QObject* undoStack, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath);

QList<QAbstractButton*> createModeMenuButtons(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath);

QAbstractButton* createImportMenuButton(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath);

QAbstractButton* createDataExportMenuButton(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath);

QAbstractButton* createPrintExportMenuButton(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath);

QAbstractButton* createSaveMenuButton(QObject* interactor, const QString& iconsDirectoryPath);

QAbstractButton* createAutoLayoutMenuButton(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath);

QAbstractButton* createNewNetworkCanvasButton(QObject* interactor, const QString& iconsDirectoryPath);

QList<QAbstractButton*> createElementStyleButtons(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath);

QMenu* createCategoryMenu(QObject* interactor, QList<MyPluginItemBase*> nodeStylesOfCategory, QList<MyPluginItemBase*> edgeStylesOfCategory, QList<MyPluginItemBase*> templateStylesOfCategory);

QObject* createElementStyleWidgetAction(QList<MyPluginItemBase*> elementStyles, QWidget* parent);

QAbstractButton* createPluginItemToolButton(QMenu* subMenu, const QString& text);

QAbstractButton* createNormalModeMenuButton(QObject* interactor, const QString& iconsDirectoryPath);

QList<QAbstractButton*> createFrequentlyUsedMenuButtons(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath);

QList<QAbstractButton*> createFrequentlyUsedNodeStyleMenuButtons(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath);

QList<QAbstractButton*> createFrequentlyUsedEdgeAndTemplateStyleMenuButtons(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath);

QList<QAbstractButton*> createAddElementMenuButtons(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath);

QAbstractButton* createUndoActionMenuButton(QObject* interactor, QObject* undoStack, const QString& iconsDirectoryPath);

QAbstractButton* createRedoActionMenuButton(QObject* interactor, QObject* undoStack, const QString& iconsDirectoryPath);

QAbstractButton* createCloseFeatureMenuButton(QObject* featureMenu, const QString& iconsDirectoryPath);

QList<QAbstractButton*> createToolbarMenuButtons(QObject* graphicsView, const QString& iconsDirectoryPath);

QAbstractButton* createZoomInMenuButton(QObject* graphicsView, const QString& iconsDirectoryPath);

QAbstractButton* createZoomOutMenuButton(QObject* graphicsView, const QString& iconsDirectoryPath);


#endif
