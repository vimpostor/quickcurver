#pragma once

#include <qqml.h>

class Backend : public QObject {
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON

	Q_PROPERTY(bool isMobile READ is_mobile CONSTANT)
public:
	bool is_mobile();
};
