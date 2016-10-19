#include "wallnode.h"

wallNode::wallNode(QSGNode *node, int fieldsize) {
	geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
	geometry->setLineWidth(WALLTHICKNESS);
	geometry->setDrawingMode(GL_LINE_STRIP);
	this->setGeometry(geometry);
	this->setFlag(QSGNode::OwnsGeometry);
	QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
	material->setColor("cyan");
	this->setMaterial(material);
	this->setFlag(QSGNode::OwnsMaterial);
	geometry->allocate(4);
	QSGGeometry::Point2D* vertices = geometry->vertexDataAsPoint2D();
	vertices[0].set(0,0);
    vertices[1].set(0,fieldsize);
    vertices[2].set(fieldsize,fieldsize);
    vertices[3].set(fieldsize,0);
	this->markDirty(QSGNode::DirtyGeometry);
	node->appendChildNode(this);
}
