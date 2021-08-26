TEMPLATE = app

QT += gui qml quick svg quickcontrols2
CONFIG += c++17

RESOURCES += qml/qml.qrc ../fluid/src/imports/controls/icons.qrc ../icons.qrc

!versionAtLeast(QT_VERSION, 5.15):error("At least Qt 5.15 is required")

CONFIG(debug, debug|release) {
	message("Running debug build")
}

CONFIG(release, debug|release) {
	message("Running release build")
	CONFIG += qtquickcompiler
}

QML_IMPORT_PATH = $$OUT_PWD/../fluid/qml

DEFINES += QT_DEPRECATED_WARNINGS

unix:!android {
	LIBS += -lrt
}

android {
	# Bundle Fluid QML plugins with the application
	ANDROID_EXTRA_PLUGINS = $$OUT_PWD/../fluid/qml

	# Android package sources
	ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../android
}

macx {
	# Bundle Fluid QML plugins with the application
	APP_QML_FILES.files = $$OUT_PWD/../fluid/qml/Fluid
	APP_QML_FILES.path = Contents/MacOS
	QMAKE_BUNDLE_DATA += APP_QML_FILES
}

ios {
	# Bundle Fluid QML plugins with the application
	APP_QML_FILES_Core.files = $$files($$OUT_PWD/../fluid/qml/Fluid/Core/*qml*)
	APP_QML_FILES_Core.path = qml/Fluid/Core
	APP_QML_FILES_Controls.files = $$files($$OUT_PWD/../fluid/qml/Fluid/Controls/*qml*) $$OUT_PWD/../fluid/qml/Fluid/Controls/icons
	APP_QML_FILES_Controls.path = qml/Fluid/Controls
	APP_QML_FILES_ControlsPrivate.files = $$files($$OUT_PWD/../fluid/qml/Fluid/Controls/Private/*qml*)
	APP_QML_FILES_ControlsPrivate.path = qml/Fluid/Controls/Private
	APP_QML_FILES_Effects.files = $$files($$OUT_PWD/../fluid/qml/Fluid/Effects/*qml*)
	APP_QML_FILES_Effects.path = qml/Fluid/Effects
	APP_QML_FILES_Layouts.files = $$files($$OUT_PWD/../fluid/qml/Fluid/Layouts/*qml*)
	APP_QML_FILES_Layouts.path = qml/Fluid/Layouts
	APP_QML_FILES_Templates.files = $$files($$OUT_PWD/../fluid/qml/Fluid/Templates/*qml*)
	APP_QML_FILES_Templates.path = qml/Fluid/Templates
	QMAKE_BUNDLE_DATA += APP_QML_FILES_Core APP_QML_FILES_Controls APP_QML_FILES_ControlsPrivate APP_QML_FILES_Effects APP_QML_FILES_Layouts APP_QML_FILES_Templates
}

win32 {
	WINDEPLOYQT_OPTIONS = -qmldir $$OUT_PWD/../fluid/qml/Fluid
}

qtConfig(static) {
	QMAKE_LIBDIR += \
		$$OUT_PWD/../fluid/qml/Fluid/Core \
		$$OUT_PWD/../fluid/qml/Fluid/Controls \
		$$OUT_PWD/../fluid/qml/Fluid/Private \
		$$OUT_PWD/../fluid/qml/Fluid/Templates
	QTPLUGIN += \
		qsvg \
		fluidcoreplugin \
		fluidcontrolsplugin \
		fluidcontrolsprivateplugin \
		fluidtemplatesplugin
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TARGET = QuickCurver

SOURCES += $$files(*.cpp, true)

HEADERS += $$files(*.h, true)
