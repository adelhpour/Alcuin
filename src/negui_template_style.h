#ifndef __NEGUI_TEMPLATE_STYLE_H
#define __NEGUI_TEMPLATE_STYLE_H

#include "negui_network_element_style_base.h"

class MyTemplateStyle : public MyNetworkElementStyleBase {
    Q_OBJECT
    
public:
    
    MyTemplateStyle(const QString& name);
    
    const QString type() const override;
    
    MyNetworkElementStyleBase* intermediaryNodeStyle();
    
    void deleteIntermediaryNodeStyle();
    
    QList<MyNetworkElementStyleBase*> sourceEdgeStyles();
    
    void clearSourceEdgeStyles();
    
    QList<MyNetworkElementStyleBase*> targetEdgeStyles();
    
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
    MyNetworkElementStyleBase* _intermediaryNodeStyle;
    QList<MyNetworkElementStyleBase*> _sourceEdgeStyles;
    QList<MyNetworkElementStyleBase*> _targetEdgeStyles;
};

#endif
