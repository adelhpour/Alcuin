#ifndef __NEGUI_ELEMENT_BASE_H
#define __NEGUI_ELEMENT_BASE_H

#include "negui_scene_mode_element_base.h"
#include "negui_base.h"
#include "negui_element_graphics_item_base.h"
#include "negui_element_style_base.h"

class MyElementBase : public QObject, public MySceneModeElementBase, public MyBase{
    Q_OBJECT
    
public:
    
    typedef enum {
        NODE_ELEMENT,
        EDGE_ELEMENT,
        ARROW_HEAD_ELEMENT,
    } ELEMENT_TYPE;
    
    MyElementBase(const QString& name);
    
    virtual ELEMENT_TYPE type() = 0;
    
    MyElementGraphicsItemBase* graphicsItem();
    
    virtual void updateGraphicsItem();
    
    MyElementStyleBase* style();
    
    virtual void setStyle(MyElementStyleBase* style);
    
    void updateStyle(QList<MyShapeStyleBase*> shapeStyles);
    
    const QString styleCategory();
    
    virtual bool setActive(const bool& active);
    
    const bool isActive() const { return _isActive; }
    
    virtual void setSelected(const bool& selected);
    
    const bool isSelected();
    
    void enableNormalMode() override;
    
    void enableAddNodeMode() override;
    
    void enableSelectNodeMode() override;
    
    void enableAddEdgeMode() override;
    
    void enableSelectEdgeMode() override;
    
    void enableRemoveMode() override;
    
    virtual const QRectF getExtents() = 0;
    
    virtual QWidget* getFeatureMenu();
    
    virtual const qint32 calculateZValue() = 0;
    
signals:
    
    void elementObject(MyElementBase*);
    
    void askForCreateChangeStageCommand();

    void askForDisplayFeatureMenu(QWidget*);
    
protected slots:
    
    void createFeatureMenu();
    
protected:
    
    MyElementGraphicsItemBase* _graphicsItem;
    MyElementStyleBase* _style;
    bool _isActive;
    bool _isSelected;
};

#endif
