#ifndef __NEGUI_ELEMENT_STYLE_BASE_H
#define __NEGUI_ELEMENT_STYLE_BASE_H

#include "negui_plugin_item_base.h"
#include "negui_shape_style_base.h"
#include "negui_element_graphics_item_base.h"

class MyElementStyleBase : public QObject, public MyPluginItemBase{
    Q_OBJECT
    
public:
    
    MyElementStyleBase(const QString& name);
    
    const QString& convertibleParentCategory() const;
    
    bool isConvertibleToParentCategory(QList<QString> parentCategories);
    
    void convertToParentCategory();
    
    QList<QString> parentCategories();
    
    virtual bool isConnectableToStartNodeCategory(const QString& connectedStartNodeCategory);
    
    virtual bool isConnectableToEndNodeCategory(const QString& connectedEndNodeCategory);
    
    QList<MyShapeStyleBase*>& shapeStyles();
    
    virtual void addDefaultShapeStyle() = 0;
    
    virtual MyShapeStyleBase* createShapeStyle(const QString& shape) = 0;
    
    void clearShapeStyles();
    
    const QRectF getShapesExtents();
    
    // get the icon associated with this plugin
    const QIcon icon() override;
    
    // get the icon graphics items of the element
    virtual QList<MyElementGraphicsItemBase*> getElementIconGraphicsItems() = 0;
    
    // read the element style info from the json object
    virtual void read(const QJsonObject &json) override;
    
    // write the element style info to the json object
    virtual void write(QJsonObject &json) override;
    
public slots:
    
    void setShapeStyles(QList<MyShapeStyleBase*> shapeStyles);
    
protected:
    QList<MyShapeStyleBase*> _shapeStyles;
    QString _convertibleParentCategory;
    QList<QString> _parentCategories;
};

#endif
