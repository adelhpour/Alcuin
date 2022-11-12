#ifndef __NEGUI_ARROW_HEAD_H
#define __NEGUI_ARROW_HEAD_H

#include "negui_element_base.h"

class MyArrowHead : public MyElementBase {
public:
    
    MyArrowHead(const QString& name, MyElementBase* edge);
    
    ~MyArrowHead();
    
    ELEMENT_TYPE type() override;
    
    const QString typeAsString() const override;
    
    // get the edge this arrow head belongs to
    MyElementBase* edge();
    
    void setSelected(const bool& selected) override;
    
    const qint32 calculateZValue() override;
    
    // read the arrow head info from the json object
    void read(const QJsonObject &json) override;
    
    // write the arrow head info to the json object
    void write(QJsonObject &json) override;
    
private slots:
    
    void setShapeStyles(QList<MyShapeStyleBase*> shapeStyles) override;
    
    QWidget* getFeatureMenu() override;
    
protected:
    MyElementBase* _edge;
};

#endif
