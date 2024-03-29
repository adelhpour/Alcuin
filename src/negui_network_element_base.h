#ifndef __NEGUI_NETWORK_ELEMENT_BASE_H
#define __NEGUI_NETWORK_ELEMENT_BASE_H

#include "negui_base.h"
#include "negui_scene_mode_element_base.h"
#include "negui_network_element_graphics_item_base.h"
#include "negui_network_element_style_base.h"

class MyNetworkElementBase : public QObject, public MySceneModeElementBase, public MyBase{
    Q_OBJECT
    
public:
    
    typedef enum {
        NODE_ELEMENT,
        EDGE_ELEMENT,
        ARROW_HEAD_ELEMENT,
    } ELEMENT_TYPE;

    MyNetworkElementBase(const QString& name);

    ~MyNetworkElementBase();
    
    virtual ELEMENT_TYPE type() = 0;

    void setName(const QString& name);
    
    MyNetworkElementGraphicsItemBase* graphicsItem();
    
    virtual void updateGraphicsItem();

    virtual void connectGraphicsItem();

    void updateFocusedGraphicsItems();
    
    MyNetworkElementStyleBase* style();
    
    virtual void setStyle(MyNetworkElementStyleBase* style);
    
    void updateStyle(QList<MyShapeStyleBase*> shapeStyles);
    
    const QString styleCategory();
    
    virtual bool setActive(const bool& active);
    
    const bool isActive() const { return _isActive; }

    virtual const bool isCopyable() = 0;

    virtual const bool isCuttable() = 0;

    virtual void setSelectedWithColor(const bool& selected) = 0;
    
    void enableNormalMode() override;
    
    void enableAddNodeMode() override;
    
    void enableSelectNodeMode() override;
    
    void enableAddEdgeMode() override;
    
    void enableSelectEdgeMode() override;
    
    virtual const QRectF getExtents() = 0;

    virtual const bool canBeMoved() = 0;

    virtual void move(const qreal& dx, const qreal& dy) = 0;
    
    virtual QWidget* getFeatureMenu();

    void addSpacerItemToFeatureMenu(QWidget* featureMenu);

    void addChangeShapeStyleButtonToFeatureMenu(QWidget* featureMenu);

    void addAddRemoveShapeStyleButtonsToFeatureMenu(QWidget* featureMenu);

    virtual const qint32 calculateZValue() = 0;

    QWidget* createFeatureMenu();

signals:
    
    void askForSelectNetworkElement(MyNetworkElementBase*);

    QList<MyNetworkElementBase*> askForListOfElements();

    QList<QGraphicsItem*> askForItemsAtPosition(const QPointF&);
    
    void askForCreateChangeStageCommand();

    void askForEnableFeatureMenuDisplay();

    QWidget* askForCurrentlyBeingDisplayedFeatureMenu();

    void askForDisplayFeatureMenu(MyNetworkElementBase*);

    void askForCopyNetworkElement(MyNetworkElementBase*);

    void askForCutNetworkElement(MyNetworkElementBase*);

    void askForCopyNetworkElementStyle(MyNetworkElementStyleBase*);

    void askForPasteNetworkElementStyle(MyNetworkElementBase*);

    void askForDeleteNetworkElement(MyNetworkElementBase*);

    bool askForWhetherElementStyleIsCopied();

    bool askForWhetherAnyOtherElementsAreSelected(QList<MyNetworkElementBase*>);

    const QString askForIconsDirectoryPath();

    bool askForCheckWhetherNetworkElementNameIsAlreadyUsed(const QString&);

    void askForDisplaySceneContextMenu(const QPointF&);

    const bool askForWhetherControlModifierIsPressed();

public slots:

    virtual void setSelected(const bool& selected);

    const bool isSelected();
    
protected slots:

    virtual const bool areAnyOtherElementsSelected();

protected:
    
    MyNetworkElementGraphicsItemBase* _graphicsItem;
    MyNetworkElementStyleBase* _style;
    bool _isActive;
    bool _isSelected;
};

QString getElementUniqueName(QList<MyNetworkElementBase*> elements, const QString& defaultIdSection);

MyNetworkElementBase* findElement(QList<MyNetworkElementBase*> elements, const QString& name);

MyNetworkElementBase* findSourceNode(QList<MyNetworkElementBase*> nodes, const QJsonObject &json);

MyNetworkElementBase* findTargetNode(QList<MyNetworkElementBase*> nodes, const QJsonObject &json);

const bool whetherNetworkElementExistsInTheListOfNetworkElements(MyNetworkElementBase* networkElement, QList<MyNetworkElementBase*> networkElements);

MyNetworkElementBase* getNetworkElement(QList<MyNetworkElementBase*> networkElements, const QString& name);

QList<MyNetworkElementBase*> getNetworkElementsOfType(QList<MyNetworkElementBase*> networkElements, const QString& type);

QList<MyNetworkElementBase*> getNetworkElementsOfCategory(QList<MyNetworkElementBase*> NetworkElements, const QString& category);

QList<MyNetworkElementBase*> getNetworkElementsOfSubCategory(QList<MyNetworkElementBase*> NetworkElements, const QString& subCategory);

#endif
