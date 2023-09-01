#include "negui_network_element_aligner_builder.h"
#include "negui_network_element_aligner.h"

MyNetworkElementAlignerBase* createNetworkElementAligner(QList<MyNetworkElementBase*> nodes, QList<MyNetworkElementBase*> edges, const QString& alignType) {
    if (alignType == "Align Top")
        return new MyNodeTopAligner(nodes);
    else if (alignType == "Align Middle")
        return new MyNodeMiddleAligner(nodes);
    else if (alignType == "Align Bottom")
        return new MyNodeBottomAligner(nodes);
    else if (alignType == "Align Left")
        return new MyNodeLeftAligner(nodes);
    else if (alignType == "Align Center")
        return new MyNodeCenterAligner(nodes);
    else if (alignType == "Align Right")
        return new MyNodeRightAligner(nodes);
    else if (alignType == "Distribute Horizontally")
        return new MyNodeHorizontallyDistributeAligner(nodes);
    else if (alignType == "Distribute Vertically")
        return new MyNodeVerticallyDistributeAligner(nodes);
    else if (alignType == "Distribute on Grid")
        return new MyNodeGridDistributeAligner(nodes);

    return NULL;
}
