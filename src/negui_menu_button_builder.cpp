#include "negui_menu_button_builder.h"
#include "negui_decorate_menu_buttons.h"
#include "negui_customized_menu_button_widgets.h"

#include <QUndoStack>
#include <QWidgetAction>

QList<QAbstractButton*> createToolbarMenuButtons(QObject* interactor, QObject* undoStack, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    QList<QAbstractButton*> buttons;
    buttons.push_back(createNewNetworkCanvasButton(interactor, iconsDirectoryPath));
    if (getPluginsOfType(plugins, "importtool").size())
        buttons.push_back(createImportMenuButton(interactor, plugins, iconsDirectoryPath));
    if (getPluginsOfType(plugins, "dataexporttool").size()) {
        buttons.push_back(createSaveMenuButton(interactor, iconsDirectoryPath));
        buttons.push_back(createDataExportMenuButton(interactor, plugins, iconsDirectoryPath));
    }
    if (getPluginsOfType(plugins, "printexporttool").size())
        buttons.push_back(createPrintExportMenuButton(interactor, plugins, iconsDirectoryPath));
    if (getPluginsOfType(plugins, "autolayoutengine").size())
        buttons.push_back(createAutoLayoutMenuButton(interactor, plugins, iconsDirectoryPath));
    buttons.push_back(createUndoActionMenuButton(interactor, undoStack, iconsDirectoryPath));
    buttons.push_back(createRedoActionMenuButton(interactor, undoStack, iconsDirectoryPath));

    return buttons;
}

QList<QAbstractButton*> createModeMenuButtons(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    QList<QAbstractButton*> buttons;
    buttons.push_back(createNormalModeMenuButton(interactor, iconsDirectoryPath));
    buttons += createFrequentlyUsedNodeStyleMenuButtons(interactor, plugins, iconsDirectoryPath);
    buttons += createFrequentlyUsedEdgeAndTemplateStyleMenuButtons(interactor, plugins, iconsDirectoryPath);
    buttons += createAddElementMenuButtons(interactor, plugins, iconsDirectoryPath);

    return buttons;
}

QAbstractButton* createImportMenuButton(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    QAbstractButton* button = new MyToolButton();
    QMenu* subMenu = new MyToolButtonMenu(button);
    MyWidgetAction* importWidgetAction = new MyWidgetAction(subMenu);
    importWidgetAction->setItems(getPluginsOfType(plugins, "importtool"));
    QObject::connect(importWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), interactor, SLOT(readFromFile(MyPluginItemBase*)));
    subMenu->addAction(importWidgetAction);
    ((QToolButton*)button)->setMenu(subMenu);
    decorateImportButton(button, iconsDirectoryPath);

    return button;
}

QAbstractButton* createDataExportMenuButton(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    QAbstractButton* button = new MyToolButton();
    QMenu* subMenu = new MyToolButtonMenu(button);
    MyWidgetAction* dataExportWidgetAction = new MyWidgetAction(subMenu);
    dataExportWidgetAction->setItems(getPluginsOfType(plugins, "dataexporttool"));
    QObject::connect(dataExportWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), interactor, SLOT(writeDataToFile(MyPluginItemBase*)));
    subMenu->addAction(dataExportWidgetAction);
    ((QToolButton*)button)->setMenu(subMenu);
    decorateDataExportButton(button, iconsDirectoryPath);

    return button;
}

QAbstractButton* createPrintExportMenuButton(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    QAbstractButton* button = new MyToolButton();
    QMenu* subMenu = new MyToolButtonMenu(button);
    MyWidgetAction* printExportWidgetAction = new MyWidgetAction(subMenu);
    printExportWidgetAction->setItems(getPluginsOfType(plugins, "printexporttool"));
    QObject::connect(printExportWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), interactor, SLOT(writeFigureToFile(MyPluginItemBase*)));
    subMenu->addAction(printExportWidgetAction);
    ((QToolButton*)button)->setMenu(subMenu);
    decoratePrintExportButton(button, iconsDirectoryPath);

    return button;
}

QAbstractButton* createSaveMenuButton(QObject* interactor, const QString& iconsDirectoryPath) {
    QAbstractButton* button = new MyToolButton();
    QObject::connect(button, SIGNAL(clicked()), interactor, SLOT(saveCurrentNetwork()));
    decorateSaveButton(button, iconsDirectoryPath);

    return button;
}

QAbstractButton* createAutoLayoutMenuButton(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    QAbstractButton* button = new MyToolButton();
    QMenu* subMenu = new MyToolButtonMenu(button);
    MyWidgetAction* autoLayoutWidgetAction = new MyWidgetAction(subMenu);
    autoLayoutWidgetAction->setItems((getPluginsOfType(plugins, "autolayoutengine")));
    QObject::connect(autoLayoutWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), interactor, SLOT(autoLayout(MyPluginItemBase*)));
    subMenu->addAction(autoLayoutWidgetAction);
    ((QToolButton*)button)->setMenu(subMenu);
    decorateAutoLayoutButton(button, iconsDirectoryPath);

    return button;
}

QAbstractButton* createNewNetworkCanvasButton(QObject* interactor, const QString& iconsDirectoryPath) {
    QAbstractButton* button = new MyToolButton();
    QObject::connect(button, SIGNAL(clicked()), interactor, SLOT(setNewNetworkCanvas()));
    decorateNewNetworkCanvasButton(button, iconsDirectoryPath);

    return button;
}

QList<QAbstractButton*> createElementStyleButtons(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    QList<QAbstractButton*> elementStyleButtons;
    for (QString category : getPluginsCategories(plugins))
        elementStyleButtons.push_back(createPluginItemToolButton(createCategoryMenu(interactor, getPluginsOfCategory(getPluginsOfType(plugins, "nodestyle"), category), getPluginsOfCategory(getPluginsOfType(plugins, "edgestyle"), category), getPluginsOfCategory(getPluginsOfType(plugins, "templatestyle"), category)), category));

    return elementStyleButtons;
}

QMenu* createCategoryMenu(QObject* interactor, QList<MyPluginItemBase*> nodeStylesOfCategory, QList<MyPluginItemBase*> edgeStylesOfCategory, QList<MyPluginItemBase*> templateStylesOfCategory) {
    QMenu* menu = new MyToolButtonCategoryMenu();
    // node
    if (nodeStylesOfCategory.size()) {
        QObject* nodeStyleWidgetAction = createElementStyleWidgetAction(nodeStylesOfCategory, menu);
        QObject::connect(nodeStyleWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), interactor, SLOT(enableAddNodeMode(MyPluginItemBase*)));
        QObject::connect(nodeStyleWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), menu, SIGNAL(menuItemIsChosen()));
        menu->addAction((QWidgetAction*)nodeStyleWidgetAction);
    }
    if (nodeStylesOfCategory.size() && (edgeStylesOfCategory.size() || templateStylesOfCategory.size()))
        menu->addSeparator();
    // edge
    if (edgeStylesOfCategory.size()) {
        QObject* edgeStyleWidgetAction = createElementStyleWidgetAction(edgeStylesOfCategory, menu);
        QObject::connect(edgeStyleWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), interactor, SLOT(enableAddEdgeMode(MyPluginItemBase*)));
        QObject::connect(edgeStyleWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), menu, SIGNAL(menuItemIsChosen()));
        menu->addAction((QWidgetAction*)edgeStyleWidgetAction);
    }
    if (edgeStylesOfCategory.size() && templateStylesOfCategory.size())
        menu->addSeparator();
    // template
    if (templateStylesOfCategory.size()) {
        QObject* templateStyleWidgetAction = createElementStyleWidgetAction(templateStylesOfCategory, menu);
        QObject::connect(templateStyleWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), interactor, SLOT(enableAddEdgeMode(MyPluginItemBase*)));
        QObject::connect(templateStyleWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), menu, SIGNAL(menuItemIsChosen()));
        menu->addAction((QWidgetAction*)templateStyleWidgetAction);
    }

    return menu;
}

QObject* createElementStyleWidgetAction(QList<MyPluginItemBase*> elementStyles, QWidget* parent) {
    QObject* widgetAction = new MyWidgetAction(parent);
    ((MyWidgetAction*)widgetAction)->setItems(elementStyles);

    return widgetAction;
}

QAbstractButton* createPluginItemToolButton(QMenu* subMenu, const QString& text) {
    QAbstractButton* button = new MyToolButton();
    button->setText(text);
    button->setToolTip("Add " + text + " to the network");
    ((QToolButton*)button)->setMenu(subMenu);
    QObject::connect(subMenu, SIGNAL(menuItemIsChosen()), button, SIGNAL(menuItemIsChosen()));

    return button;
}

QAbstractButton* createNormalModeMenuButton(QObject* interactor, const QString& iconsDirectoryPath) {
    QAbstractButton* button = new MyModeMenuModeButton("Normal", "Display_Feature_Menu");
    decorateNormalModeButton(button, iconsDirectoryPath);
    QObject::connect(button, SIGNAL(clicked()), interactor, SLOT(enableNormalMode()));

    return button;
}

QList<QAbstractButton*> createFrequentlyUsedNodeStyleMenuButtons(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    QList<QAbstractButton*> frequentlyUsedNodeStyleMenuButtons;
    QList<MyPluginItemBase*> nodeStyles = getPluginsOfType(plugins, "nodestyle");
    for (MyPluginItemBase* nodeStyle : nodeStyles) {
        if (nodeStyle->isFrequentlyUsed()) {
            QAbstractButton* frequentlyUsedNodeStyleButton = new MyItemPreviewButton(nodeStyle);
            QObject::connect(frequentlyUsedNodeStyleButton, SIGNAL(itemIsChosen(MyPluginItemBase*)), interactor, SLOT(enableAddNodeMode(MyPluginItemBase*)));
            frequentlyUsedNodeStyleMenuButtons.push_back(frequentlyUsedNodeStyleButton);
        }
    }

    return frequentlyUsedNodeStyleMenuButtons;
}

QList<QAbstractButton*> createFrequentlyUsedEdgeAndTemplateStyleMenuButtons(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    QList<QAbstractButton*> frequentlyUsedEdgeAndTemplateStyleMenuButtons;
    QList<MyPluginItemBase*> edgeAndTemplateStyles = getPluginsOfType(plugins, "edgestyle") + getPluginsOfType(plugins, "templatestyle");
    for (MyPluginItemBase* edgeAndTemplateStyle : edgeAndTemplateStyles) {
        if (edgeAndTemplateStyle->isFrequentlyUsed()) {
            QAbstractButton* frequentlyUsedEdgeAndTemplateStyleButton = new MyItemPreviewButton(edgeAndTemplateStyle);
            QObject::connect(frequentlyUsedEdgeAndTemplateStyleButton, SIGNAL(itemIsChosen(MyPluginItemBase*)), interactor, SLOT(enableAddEdgeMode(MyPluginItemBase*)));
            frequentlyUsedEdgeAndTemplateStyleMenuButtons.push_back(frequentlyUsedEdgeAndTemplateStyleButton);
        }
    }

    return frequentlyUsedEdgeAndTemplateStyleMenuButtons;
}

QList<QAbstractButton*> createAddElementMenuButtons(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    return createElementStyleButtons(interactor, plugins, iconsDirectoryPath);
}

QAbstractButton* createUndoActionMenuButton(QObject* interactor, QObject* undoStack, const QString& iconsDirectoryPath) {
    QAction* action = ((QUndoStack*)undoStack)->createUndoAction(interactor, "Undo");

    QAbstractButton* button = new MyToolButton();
    ((QToolButton*)button)->setDefaultAction(action);
    decorateUndoActionButton(button, iconsDirectoryPath);
    QObject::connect(((QUndoStack*)undoStack), &QUndoStack::indexChanged, [button, iconsDirectoryPath] () { decorateUndoActionButton(button, iconsDirectoryPath); });
    QObject::connect(((QUndoStack*)undoStack), &QUndoStack::canUndoChanged, [button, iconsDirectoryPath] () { decorateUndoActionButton(button, iconsDirectoryPath); });
    QObject::connect(((QUndoStack*)undoStack), &QUndoStack::canRedoChanged, [button, iconsDirectoryPath] () { decorateUndoActionButton(button, iconsDirectoryPath); });

    return button;
}

QAbstractButton* createRedoActionMenuButton(QObject* interactor, QObject* undoStack, const QString& iconsDirectoryPath) {
    QAction* action = ((QUndoStack*)undoStack)->createRedoAction(interactor, "Redo");

    QAbstractButton* button = new MyToolButton();
    ((QToolButton*)button)->setDefaultAction(action);
    decorateRedoActionButton(button, iconsDirectoryPath);
    QObject::connect(((QUndoStack*)undoStack), &QUndoStack::indexChanged, [button, iconsDirectoryPath] () { decorateRedoActionButton(button, iconsDirectoryPath); });
    QObject::connect(((QUndoStack*)undoStack), &QUndoStack::canUndoChanged, [button, iconsDirectoryPath] () { decorateRedoActionButton(button, iconsDirectoryPath); });
    QObject::connect(((QUndoStack*)undoStack), &QUndoStack::canRedoChanged, [button, iconsDirectoryPath] () { decorateRedoActionButton(button, iconsDirectoryPath); });

    return button;
}

QAbstractButton* createCloseFeatureMenuButton(QObject* featureMenu, const QString& iconsDirectoryPath) {
    QAbstractButton* button = new QPushButton();
    decorateCloseFeatureMenuButton(button, iconsDirectoryPath);
    QObject::connect(button, SIGNAL(clicked()), featureMenu, SIGNAL(askForRemoveFeatureMenu()));

    return button;
}

QList<QAbstractButton*> createToolbarMenuButtons(QObject* graphicsView, const QString& iconsDirectoryPath) {
    QList<QAbstractButton*> toolbarMenuButtons;
    toolbarMenuButtons.push_back(createZoomInMenuButton(graphicsView, iconsDirectoryPath));
    toolbarMenuButtons.push_back(createZoomOutMenuButton(graphicsView, iconsDirectoryPath));

    return toolbarMenuButtons;
}

QAbstractButton* createZoomInMenuButton(QObject* graphicsView, const QString& iconsDirectoryPath) {
    QAbstractButton* button = new MyToolButton();
    decorateZoomInButton(button, iconsDirectoryPath);
    QObject::connect(button, SIGNAL(clicked()), graphicsView, SLOT(zoomIn()));
    return button;
}

QAbstractButton* createZoomOutMenuButton(QObject* graphicsView, const QString& iconsDirectoryPath) {
    QAbstractButton* button = new MyToolButton();
    decorateZoomOutButton(button, iconsDirectoryPath);
    QObject::connect(button, SIGNAL(clicked()), graphicsView, SLOT(zoomOut()));
    return button;
}