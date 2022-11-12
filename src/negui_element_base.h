#ifndef __NEGUI_ELEMENT_BASE_H
#define __NEGUI_ELEMENT_BASE_H

#include "negui_base.h"
#include "negui_element_graphics_item_base.h"
#include "negui_element_style_base.h"

class MyElementBase : public QObject, public MyBase {
    Q_OBJECT
    
public:
    
    typedef enum {
        NODE_ELEMENT,
        EDGE_ELEMENT,
        ARROW_HEAD_ELEMENT,
    } ELEMENT_TYPE;
    
    MyElementBase(const QString& name);
    
    virtual ELEMENT_TYPE type() = 0;
    
    virtual const QString typeAsString() const = 0;
    
    MyElementGraphicsItemBase* graphicsItem();
    
    virtual void updateGraphicsItem();
    
    MyElementStyleBase* style();
    
    void setStyle(MyElementStyleBase* style);
    
    const QString styleCategory();
    
    bool isStyleCategoryConvertibleToParentCategory();
    
    void convertStyleCategoryToParentCategory();
    
    virtual bool setActive(const bool& active);
    
    const bool isActive() const { return _isActive; }
    
    virtual void setSelected(const bool& selected) = 0;
    
    const bool isSelected();
    
    virtual void enableNormalMode();
    
    virtual void enableAddNodeMode();
    
    virtual void enableSelectNodeMode();
    
    virtual void enableAddEdgeMode();
    
    virtual void enableSelectEdgeMode();
    
    virtual void enableRemoveMode();
    
    virtual const qint32 calculateZValue() = 0;
    
signals:
    
    void elementObject(MyElementBase*);
    
protected slots:
    
    virtual void setShapeStyles(QList<MyShapeStyleBase*> shapeStyles);
    
    virtual QWidget* getFeatureMenu();
    
protected:
    
    MyElementGraphicsItemBase* _graphicsItem;
    MyElementStyleBase* _style;
    bool _isActive;
};

#endif
