#ifndef __NEGUI_TEMPLATE_STYLE_H
#define __NEGUI_TEMPLATE_STYLE_H

#include "negui_element_style_base.h"

class MyTemplateStyle : public MyElementStyleBase {
    Q_OBJECT
    
public:
    
    MyTemplateStyle(const QString& name);
    
    const QString type() const override;
    
    MyElementStyleBase* intermediaryNodeStyle();
    
    void deleteIntermediaryNodeStyle();
    
    QList<MyElementStyleBase*> sourceEdgeStyles();
    
    void clearSourceEdgeStyles();
    
    QList<MyElementStyleBase*> targetEdgeStyles();
    
    void clearTargetEdgeStyles();
    
    void addDefaultShapeStyle() override;
    
    MyShapeStyleBase* createShapeStyle(const QString& shape) override;
    
    void clearShapeStyles() override;
    
    const QRectF getShapesExtents(QRectF defaultExtents = QRectF(0.0, 0.0, 0.0, 0.0)) override;
    
    QObject* createIconBuilder() override;
    
    const QString toolTipText() override;
    
    void read(const QJsonObject &json) override;
    
    void write(QJsonObject &json) override;

protected:
    MyElementStyleBase* _intermediaryNodeStyle;
    QList<MyElementStyleBase*> _sourceEdgeStyles;
    QList<MyElementStyleBase*> _targetEdgeStyles;
};

#endif
