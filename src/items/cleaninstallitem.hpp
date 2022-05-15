#ifndef CLEANINSTALLITEM_H
#define CLEANINSTALLITEM_H

#include "item.hpp"

/**
 * @brief Cleaninstalls the entire round
 *
 * Erases every line
 */
class CleanInstallItem : public Item
{
public:
	CleanInstallItem(QSGNode *parentNode, QString iconName, AllowedUsers allowedUsers, QPointF pos);
private:
	virtual void use(Curver *curver) override;
};

#endif // CLEANINSTALLITEM_H
