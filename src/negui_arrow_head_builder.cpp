#include "negui_arrow_head_builder.h"
#include "negui_arrow_head.h"
#include "negui_arrow_head_style.h"

MyNetworkElementBase* createArrowHead(const QString& name, MyElementStyleBase* arrowHeadStyle, MyNetworkElementBase* edge) {
    MyNetworkElementBase* arrowHead = NULL;
    if (((MyArrowHeadStyleBase*)arrowHeadStyle)->arrowHeadStyleType() == MyArrowHeadStyleBase::CLASSIC_ARROW_HEAD_STYLE)
        arrowHead = new MyClassicArrowHead(name, edge);

    if (arrowHead)
        arrowHead->setStyle(arrowHeadStyle);

    return arrowHead;
}
