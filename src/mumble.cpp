#ifdef MUMBLE_SUPPORT

#include "mumble.h"

#include <QDebug>
#include <unistd.h>
#include <stdio.h>
#include <wchar.h>

#define IDLE_INTERVAL 512

/**
 * @brief Constructs a Mumble Api instance
 */
Mumble::Api::Api()
{
	connect(&idleTimer, &QTimer::timeout, this, &Mumble::Api::increaseTick);
	idleTimer.setInterval(IDLE_INTERVAL);
}

/**
 * @brief Returns a singleton Mumble Api instance
 * @return The singleton instance
 */
Mumble::Api* Mumble::Api::get()
{
	static Api result;
	return &result;
}

/**
 * @brief Tries to connect to Mumble and to retreive a handle
 */
void Mumble::Api::initialize()
{
	initializeHandle();
	if (lm) {
		if (lm->uiVersion != 2) {
			wcsncpy(lm->name, L"QuickCurver", 256);
			wcsncpy(lm->description, L"Supports Positional Audio for QuickCurver.", 2048);
			lm->uiVersion = 2;
		}
		setGeneralInfo("NULL", "NULL");
		idleTimer.start();
	}
}

/**
 * @brief Sets general Mumble data
 * @param identity The identity of the player
 * @param context The context of the player
 */
void Mumble::Api::setGeneralInfo(QString identity, QString context)
{
	if (!lm) {
		return;
	}
	lm->fAvatarFront[0] = lm->fCameraFront[0] = 0.0f;
	lm->fAvatarFront[1] = lm->fCameraFront[1] = 0.0f;
	lm->fAvatarFront[2] = lm->fCameraFront[2] = 1.0f;
	lm->fAvatarTop[0] = lm->fCameraTop[0] = 0.0f;
	lm->fAvatarTop[1] = lm->fCameraTop[1] = 1.0f;
	lm->fAvatarTop[2] = lm->fCameraTop[2] = 0.0f;
	lm->fAvatarPosition[0] = lm->fCameraPosition[0] = 0.0f;
	lm->fAvatarPosition[1] = lm->fCameraPosition[1] = 0.0f;
	lm->fAvatarPosition[2] = lm->fCameraPosition[2] = 0.0f;
	wchar_t id[identity.size()];
	identity.toWCharArray(id);
	wcsncpy(lm->identity, id, identity.size());
	memcpy(lm->context, context.toStdString().c_str(), context.size());
	lm->context_len = context.size();
	increaseTick();
}

/**
 * @brief Propagates the position to Mumble
 * @param pos The position of the player
 */
void Mumble::Api::updatePosition(const QPointF pos)
{
	if (!lm) {
		return;
	}
	auto correctedPos = pos / 100;
	correctedPos.setY(-1 * correctedPos.y());
	lm->fAvatarPosition[0] = lm->fCameraPosition[0] = correctedPos.x();
	lm->fAvatarPosition[1] = lm->fCameraPosition[1] = 0.0f;
	lm->fAvatarPosition[2] = lm->fCameraPosition[2] = correctedPos.y();
	increaseTick();
}

/**
 * @brief Increases the tick in a thread-safe manner
 */
void Mumble::Api::increaseTick()
{
	tickLock.lock();
	lm->uiTick++;
	tickLock.unlock();
}

/**
 * @brief Initializes the \a lm variable
 *
 * If initialization failed, \a lm will be \c nullptr
 */
void Mumble::Api::initializeHandle()
{
#ifdef _WIN32
	HANDLE hMapObject = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, L"MumbleLink");
	if (hMapObject == nullptr)
		return;

	lm = (LinkedMem *) MapViewOfFile(hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(LinkedMem));
	if (lm == nullptr) {
		CloseHandle(hMapObject);
		hMapObject = nullptr;
		return;
	}
#else
	char memname[256];
	snprintf(memname, 256, "/MumbleLink.%d", getuid());

	int shmfd = shm_open(memname, O_RDWR, S_IRUSR | S_IWUSR);

	if (shmfd < 0) {
		return;
	}

	lm = static_cast<LinkedMem*>((mmap(nullptr, sizeof(struct LinkedMem), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd,0)));

	if (lm == (void *)(-1)) {
		lm = nullptr;
		return;
	}
#endif
	if (lm != nullptr) {
		qDebug() << "Connected to Mumble";
	}
}

#endif // MUMBLE_SUPPORT
