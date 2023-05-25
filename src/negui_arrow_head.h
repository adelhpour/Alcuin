#ifndef __NEGUI_ARROW_HEAD_H
#define __NEGUI_ARROW_HEAD_H

#include "negui_network_element_base.h"

class MyArrowHeadBase : public MyNetworkElementBase {

public:

    typedef enum {
        CLASSIC_ARROW_HEAD,
    } ARROW_HEAD_TYPE;

    MyArrowHeadBase(const QString& name, MyNetworkElementBase* edge);
    
    ~MyArrowHeadBase();
    
    ELEMENT_TYPE type() override;

    virtual ARROW_HEAD_TYPE arrowHeadType() = 0;
    
    // get the edge this arrow head belongs to
    MyNetworkElementBase* edge();
    
    void updateGraphicsItem() override;
    
    void updatePlacement(const QPointF& position, const qreal& slope);
    
    void setSelected(const bool& selected) override;
    
    const QRectF getExtents() override;

    QWidget* getFeatureMenu() override;
    
    const qint32 calculateZValue() override;
    
    // read the arrow head info from the json object
    void read(const QJsonObject &json) override;
    
    // write the arrow head info to the json object
    void write(QJsonObject &json) override;
    
protected:
    MyNetworkElementBase* _edge;
    QPointF _position;
    qreal _slope;
};

class MyClassicArrowHead : public MyArrowHeadBase {
    Q_OBJECT

public:

    MyClassicArrowHead(const QString& name, MyNetworkElementBase* edge);

    ARROW_HEAD_TYPE arrowHeadType() override;

    QWidget* getFeatureMenu() override;
};

#endif
