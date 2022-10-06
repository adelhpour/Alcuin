#ifndef __NEGUI_ARROW_HEAD_H
#define __NEGUI_ARROW_HEAD_H

#include "negui_base.h"
#include "negui_element_base.h"
#include "negui_edge.h"
#include "negui_arrow_head_graphics_item.h"

class MyEdge;

class MyArrowHead : public MyElementBase {
public:
    
    MyArrowHead(const QString& name, MyEdge* edge);
    
    ~MyArrowHead();
    
    ELEMENT_TYPE type() override;
    
    const QString typeAsString() const override;
    
    // get the edge this arrow head belongs to
    MyEdge* edge();
    
    void setSelected(const bool& selected) override;
    
    // enable the normal mode of the arrow head
    void enableNormalMode() override;
    
    // enable the add node mode of the arrow head
    void enableAddNodeMode() override;
    
    void enableSelectNodeMode() override;
    
    // enable the add edge mode of the arrow head
    void enableAddEdgeMode() override;
    
    void enableSelectEdgeMode() override;
    
    // enable the remove mode of the arrow head
    void enableRemoveMode() override;
    
    const qint32 calculateZValue() override;
    
    // read the arrow head info from the json object
    void read(const QJsonObject &json) override;
    
    // write the arrow head info to the json object
    void write(QJsonObject &json) override;
    
private slots:
    
    void setShapeStyles(QList<MyShapeStyleBase*> shapeStyles) override;
    
    QWidget* getFeatureMenu() override;
    
protected:
    MyEdge* _edge;
};

#endif
