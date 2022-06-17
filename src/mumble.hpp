#ifdef MUMBLE_SUPPORT

#ifndef MUMBLE_H
#define MUMBLE_H

#include <QMutex>
#include <QPointF>
#include <QString>
#include <QTimer>
#if defined(_WIN32)
#include <windows.h>
#elif defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
#include <fcntl.h> /* For O_* constants */
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#endif // _WIN32

namespace Mumble {

struct LinkedMem {
#ifdef _WIN32
	UINT32 uiVersion;
	DWORD uiTick;
#else
	uint32_t uiVersion;
	uint32_t uiTick;
#endif
	float fAvatarPosition[3];
	float fAvatarFront[3];
	float fAvatarTop[3];
	wchar_t name[256];
	float fCameraPosition[3];
	float fCameraFront[3];
	float fCameraTop[3];
	wchar_t identity[256];
#ifdef _WIN32
	UINT32 context_len;
#else
	uint32_t context_len;
#endif
	unsigned char context[256];
	wchar_t description[2048];
};

/**
 * @brief A Mumble Api Wrapper
 */
class Api : public QObject {
	Q_OBJECT
public:
	Api();
	static Api *get();

	void initialize();
	void setGeneralInfo(QString identity, QString context);
	void updatePosition(const QPointF pos);
private:
	void increaseTick();
	void initializeHandle();
	/**
	 * @brief A handle to the linked Mumble memory
	 */
	LinkedMem *lm = nullptr;
	/**
	 * @brief A timer, that tells Mumble that we are still there
	 */
	QTimer idleTimer;
	/**
	 * @brief A mutex allowing the thread-safe usage of the tick variable
	 */
	QMutex tickLock;
};

}


#endif // MUMBLE_H

#endif // MUMBLE_SUPPORT
