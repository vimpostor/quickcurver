#include "headnode.h"

headNode::headNode(QPointF startPos, QSGFlatColorMaterial* material, QSGNode *node) {
	pos = startPos;
	geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
	geometry->setLineWidth(thickness);
	geometry->setDrawingMode(GL_POINTS);
	this->setGeometry(geometry);
	this->setFlag(QSGNode::OwnsGeometry);
	this->setMaterial(material);
	this->setFlag(QSGNode::OwnsMaterial);
	geometry->allocate(1);
	QSGGeometry::Point2D* vertices = geometry->vertexDataAsPoint2D();
	vertices[0].set(pos.x(),pos.y());
	this->markDirty(QSGNode::DirtyGeometry);
	node->appendChildNode(this);
}

void headNode::updatePosition(QPointF newPos) {
	pos = newPos;
	QSGGeometry::Point2D* vertices = geometry->vertexDataAsPoint2D();
	vertices[0].set(pos.x(),pos.y());
	this->markDirty(QSGNode::DirtyGeometry);
}

void headNode::setThickness(int newThickness) {
	thickness = 2*newThickness - 2;
	geometry->setLineWidth(thickness);
}
