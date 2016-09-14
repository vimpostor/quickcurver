#include "graphnode.h"

GraphNode::GraphNode() {
}

void GraphNode::addChildNode(QSGNode* node) {
	this->appendChildNode(node);
}
