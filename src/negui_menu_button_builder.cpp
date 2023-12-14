#include "negui_menu_button_builder.h"
#include "negui_decorate_menu_buttons.h"
#include "negui_customized_menu_button_widgets.h"

#include <QUndoStack>
#include <QWidgetAction>

QList<QAbstractButton*> createToolbarMenuButtons(QObject* interactor, QObject* undoStack, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    QList<QAbstractButton*> buttons;
    buttons.push_back(createNewNetworkCanvasButton(interactor, iconsDirectoryPath));
    if (getPluginsOfType(plugins, "open").size())
        buttons.push_back(createOpenMenuButton(interactor, plugins, iconsDirectoryPath));
    if (getPluginsOfType(plugins, "save_as").size()) {
        buttons.push_back(createSaveMenuButton(interactor, iconsDirectoryPath));
        buttons.push_back(createSaveAsMenuButton(interactor, plugins, iconsDirectoryPath));
    }
    if (getPluginsOfType(plugins, "export").size())
        buttons.push_back(createExportMenuButton(interactor, plugins, iconsDirectoryPath));
    buttons.push_back(createUndoActionMenuButton(interactor, undoStack, iconsDirectoryPath));
    buttons.push_back(createRedoActionMenuButton(interactor, undoStack, iconsDirectoryPath));
    if (getPluginsOfType(plugins, "default").size())
        buttons.push_back(createMoreMenuButton(interactor, plugins, iconsDirectoryPath));

    return buttons;
}

QList<QAbstractButton*> createModeMenuButtons(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    QList<QAbstractButton*> buttons;
    buttons.push_back(createNormalModeMenuButton(interactor, iconsDirectoryPath));
    buttons += createFrequentlyUsedNodeStyleMenuButtons(interactor, plugins, iconsDirectoryPath);
    buttons += createFrequentlyUsedEdgeAndTemplateStyleMenuButtons(interactor, plugins, iconsDirectoryPath);
    buttons.push_back(createExtraElementMenuButton(interactor, plugins, iconsDirectoryPath));

    return buttons;
}

QAbstractButton* createOpenMenuButton(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    QAbstractButton* button = new MyToolButton();
    QMenu* subMenu = new MyMenu(button);
    MyMenuButtonWidgetAction* openItemWidgetAction = new MyMenuButtonWidgetAction(subMenu);
    openItemWidgetAction->setItems(getPluginsOfType(plugins, "open"));
    QObject::connect(openItemWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), interactor, SLOT(callPluginFunctions(MyPluginItemBase*)));
    subMenu->addAction(openItemWidgetAction);
    ((QToolButton*)button)->setMenu(subMenu);
    decorateOpenButton(button, iconsDirectoryPath);

    return button;
}

QAbstractButton* createSaveAsMenuButton(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    QAbstractButton* button = new MyToolButton();
    QMenu* subMenu = new MyMenu(button);
    MyMenuButtonWidgetAction* saveAsItemWidgetAction = new MyMenuButtonWidgetAction(subMenu);
    saveAsItemWidgetAction->setItems(getPluginsOfType(plugins, "save_as"));
    QObject::connect(saveAsItemWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), interactor, SLOT(callPluginFunctions(MyPluginItemBase*)));
    subMenu->addAction(saveAsItemWidgetAction);
    ((QToolButton*)button)->setMenu(subMenu);
    decorateSaveAsButton(button, iconsDirectoryPath);

    return button;
}

QAbstractButton* createExportMenuButton(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    QAbstractButton* button = new MyToolButton();
    QMenu* subMenu = new MyMenu(button);
    MyMenuButtonWidgetAction* exportItemWidgetAction = new MyMenuButtonWidgetAction(subMenu);
    exportItemWidgetAction->setItems(getPluginsOfType(plugins, "export"));
    QObject::connect(exportItemWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), interactor, SLOT(callPluginFunctions(MyPluginItemBase*)));
    subMenu->addAction(exportItemWidgetAction);
    ((QToolButton*)button)->setMenu(subMenu);
    decorateExportButton(button, iconsDirectoryPath);

    return button;
}

QAbstractButton* createSaveMenuButton(QObject* interactor, const QString& iconsDirectoryPath) {
    QAbstractButton* button = new MyToolButton();
    QObject::connect(button, SIGNAL(clicked()), interactor, SLOT(saveCurrentNetwork()));
    decorateSaveButton(button, iconsDirectoryPath);

    return button;
}

QAbstractButton* createNewNetworkCanvasButton(QObject* interactor, const QString& iconsDirectoryPath) {
    QAbstractButton* button = new MyToolButton();
    QObject::connect(button, SIGNAL(clicked()), interactor, SLOT(setNewNetworkCanvas()));
    decorateNewNetworkCanvasButton(button, iconsDirectoryPath);

    return button;
}

QList<QAbstractButton*> createElementCategoryMenuButtons(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    QList<QAbstractButton*> elementCateogryMenuButtons;
    for (QString category : getPluginsCategories(plugins))
        elementCateogryMenuButtons.push_back(new MyExtraElementCategoryButton(createElementCategoryMenu(interactor, getPluginsOfCategory(getPluginsOfType(plugins, "nodestyle"), category), getPluginsOfCategory(getPluginsOfType(plugins, "edgestyle"), category), getPluginsOfCategory(getPluginsOfType(plugins, "templatestyle"), category)), category));

    return elementCateogryMenuButtons;
}

QMenu* createElementCategoryMenu(QObject* interactor, QList<MyPluginItemBase*> nodeStylesOfCategory, QList<MyPluginItemBase*> edgeStylesOfCategory, QList<MyPluginItemBase*> templateStylesOfCategory) {
    QMenu* extraElementCategoryMenu = new MyExtraElementCategoryMenu();
    // node
    if (nodeStylesOfCategory.size()) {
        QObject* nodeStyleWidgetAction = createElementStyleWidgetAction(nodeStylesOfCategory, extraElementCategoryMenu);
        QObject::connect(nodeStyleWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), interactor, SLOT(enableAddNodeMode(MyPluginItemBase*)));
        QObject::connect(nodeStyleWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), extraElementCategoryMenu, SIGNAL(categoryItemIsChosen()));
        extraElementCategoryMenu->addAction((QWidgetAction*)nodeStyleWidgetAction);
    }
    if (nodeStylesOfCategory.size() && (edgeStylesOfCategory.size() || templateStylesOfCategory.size()))
        extraElementCategoryMenu->addSeparator();
    // edge
    if (edgeStylesOfCategory.size()) {
        QObject* edgeStyleWidgetAction = createElementStyleWidgetAction(edgeStylesOfCategory, extraElementCategoryMenu);
        QObject::connect(edgeStyleWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), interactor, SLOT(enableAddEdgeMode(MyPluginItemBase*)));
        QObject::connect(edgeStyleWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), extraElementCategoryMenu, SIGNAL(categoryItemIsChosen()));
        extraElementCategoryMenu->addAction((QWidgetAction*)edgeStyleWidgetAction);
    }
    if (edgeStylesOfCategory.size() && templateStylesOfCategory.size())
        extraElementCategoryMenu->addSeparator();
    // template
    if (templateStylesOfCategory.size()) {
        QObject* templateStyleWidgetAction = createElementStyleWidgetAction(templateStylesOfCategory, extraElementCategoryMenu);
        QObject::connect(templateStyleWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), interactor, SLOT(enableAddEdgeMode(MyPluginItemBase*)));
        QObject::connect(templateStyleWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), extraElementCategoryMenu, SIGNAL(categoryItemIsChosen()));
        extraElementCategoryMenu->addAction((QWidgetAction*)templateStyleWidgetAction);
    }

    return extraElementCategoryMenu;
}

QObject* createElementStyleWidgetAction(QList<MyPluginItemBase*> elementStyles, QWidget* parent) {
    MyMenuButtonWidgetAction* elementStyleWidgetAction = new MyMenuButtonWidgetAction(parent);
    elementStyleWidgetAction->setItems(elementStyles);

    return elementStyleWidgetAction;
}

QAbstractButton* createNormalModeMenuButton(QObject* interactor, const QString& iconsDirectoryPath) {
    QAbstractButton* button = new MyModeMenuModeButton("Normal");
    decorateNormalModeButton(button, iconsDirectoryPath);
    QObject::connect(button, SIGNAL(clicked()), interactor, SLOT(enableNormalMode()));

    return button;
}

QList<QAbstractButton*> createFrequentlyUsedNodeStyleMenuButtons(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    QList<QAbstractButton*> frequentlyUsedNodeStyleMenuButtons;
    QList<MyPluginItemBase*> nodeStyles = getPluginsOfType(plugins, "nodestyle");
    for (MyPluginItemBase* nodeStyle : nodeStyles) {
        if (nodeStyle->isFrequentlyUsed()) {
            QAbstractButton* frequentlyUsedNodeStyleButton = new MyMenuItemPreviewButton(nodeStyle);
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
            QAbstractButton* frequentlyUsedEdgeAndTemplateStyleButton = new MyMenuItemPreviewButton(edgeAndTemplateStyle);
            QObject::connect(frequentlyUsedEdgeAndTemplateStyleButton, SIGNAL(itemIsChosen(MyPluginItemBase*)), interactor, SLOT(enableAddEdgeMode(MyPluginItemBase*)));
            frequentlyUsedEdgeAndTemplateStyleMenuButtons.push_back(frequentlyUsedEdgeAndTemplateStyleButton);
        }
    }

    return frequentlyUsedEdgeAndTemplateStyleMenuButtons;
}

QAbstractButton* createExtraElementMenuButton(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    QMenu* extraElementMenu = new MyExtraElementMenu();
    QList<QAbstractButton*> elementCategoryMenuButtons = createElementCategoryMenuButtons(interactor, plugins, iconsDirectoryPath);
    for (QAbstractButton* elementCategoryMenuButton : elementCategoryMenuButtons) {
        QWidgetAction* widgetAction = new QWidgetAction(extraElementMenu);
        widgetAction->setDefaultWidget(elementCategoryMenuButton);
        extraElementMenu->addAction(widgetAction);
        extraElementMenu->addSeparator();
        QObject::connect(elementCategoryMenuButton, SIGNAL(categoryItemIsChosen()), extraElementMenu, SLOT(close()));
    }
    QAbstractButton* extraElementMenuButton = new MyModeMenuModeButton("Add_Node", "Add_Edge");
    decorateExtraElementsMenuButton(extraElementMenuButton, iconsDirectoryPath);
    ((MyToolButton*)extraElementMenuButton)->setMenu(extraElementMenu);

    return extraElementMenuButton;
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

QAbstractButton* createMoreMenuButton(QObject* interactor, QList<MyPluginItemBase*> plugins, const QString& iconsDirectoryPath) {
    QAbstractButton* button = new MyToolButton();
    QMenu* subMenu = new MyMenu(button);
    MyMenuButtonWidgetAction* defaultItemWidgetAction = new MyMenuButtonWidgetAction(subMenu);
    defaultItemWidgetAction->setItems(getPluginsOfType(plugins, "default"));
    QObject::connect(defaultItemWidgetAction, SIGNAL(itemIsChosen(MyPluginItemBase*)), interactor, SLOT(callPluginFunctions(MyPluginItemBase*)));
    subMenu->addAction(defaultItemWidgetAction);
    ((QToolButton*)button)->setMenu(subMenu);
    decorateMoreButton(button, iconsDirectoryPath);

    return button;
}