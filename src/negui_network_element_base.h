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
    
    virtual ELEMENT_TYPE type() = 0;
    
    MyNetworkElementGraphicsItemBase* graphicsItem();
    
    virtual void updateGraphicsItem();

    virtual void connectGraphicsItem();
    
    MyNetworkElementStyleBase* style();
    
    virtual void setStyle(MyNetworkElementStyleBase* style);
    
    void updateStyle(QList<MyShapeStyleBase*> shapeStyles);
    
    const QString styleCategory();
    
    virtual bool setActive(const bool& active);
    
    const bool isActive() const { return _isActive; }

    virtual const bool isCopyable() = 0;

    virtual const bool isCuttable() = 0;
    
    virtual void setSelected(const bool& selected);

    virtual void setSelectedWithColor(const bool& selected) = 0;
    
    void enableNormalMode() override;
    
    void enableAddNodeMode() override;
    
    void enableSelectNodeMode() override;
    
    void enableAddEdgeMode() override;
    
    void enableSelectEdgeMode() override;
    
    virtual const QRectF getExtents() = 0;
    
    virtual QWidget* getFeatureMenu();
    
    virtual const qint32 calculateZValue() = 0;
    
signals:
    
    void elementObject(MyNetworkElementBase*);
    
    void askForCreateChangeStageCommand();

    void askForDisplayFeatureMenu(QWidget*);

    void askForCopyNetworkElement(MyNetworkElementBase*);

    void askForCutNetworkElement(MyNetworkElementBase*);

    void askForCopyNetworkElementStyle(MyNetworkElementStyleBase*);

    void askForPasteNetworkElementStyle(MyNetworkElementBase*);

    void askForDeleteNetworkElement(MyNetworkElementBase*);

    const bool askForWhetherElementStyleIsCopied();

    const bool askForWhetherAnyOtherElementsAreSelected(MyNetworkElementBase*);

public slots:

    const bool isSelected();
    
protected slots:
    
    void createFeatureMenu();

    const bool areAnyOtherElementsSelected();
    
protected:
    
    MyNetworkElementGraphicsItemBase* _graphicsItem;
    MyNetworkElementStyleBase* _style;
    bool _isActive;
    bool _isSelected;
};

#endif
