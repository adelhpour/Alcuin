#ifndef __NEGUI_ELEMENT_ALIGNER_BUILDER_H
#define __NEGUI_ELEMENT_ALIGNER_BUILDER_H

#include "negui_network_element_aligner.h"

MyNetworkElementAlignerBase* createNetworkElementAligner(QList<MyNetworkElementBase*> nodes, QList<MyNetworkElementBase*> edges, const QString& alignType);

#endif
