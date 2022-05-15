#ifndef INVISIBLEITEM_H
#define INVISIBLEITEM_H

#include "item.hpp"

/**
 * @brief Renders a Curver invisible and makes it immune to collisions
 */
class InvisibleItem : public Item
{
public:
	InvisibleItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos);
private:
	virtual void use(Curver *curver) override;
};

#endif // INVISIBLEITEM_H
