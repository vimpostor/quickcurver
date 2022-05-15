#include "backend.hpp"

bool Backend::is_mobile() {
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
	return true;
#else
	return false;
#endif
}
