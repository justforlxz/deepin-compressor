#-------------------------------------------------
#
# Project created by QtCreator 2019-08-23T15:45:27
#
#-------------------------------------------------

QT       += core gui dtkwidget
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cli7zplugin
TEMPLATE = lib

DEFINES += CLI7ZPLUGIN_LIBRARY

CONFIG         += plugin

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../../deepin-compressor/source/inc/ \
                $$PWD/../../deepin-compressor/interface/

SOURCES += \
        cliplugin.cpp \
    ../../deepin-compressor/interface/options.cpp \
    ../../deepin-compressor/interface/queries.cpp \
    ../../deepin-compressor/interface/desktopfileparser.cpp \
    ../../deepin-compressor/interface/kpluginmetadata.cpp \
    ../../deepin-compressor/interface/pluginmanager.cpp \
    ../../deepin-compressor/interface/jobs.cpp \
    ../../deepin-compressor/interface/archive_manager.cpp \
    ../../deepin-compressor/interface/mimetypes.cpp \
    ../../deepin-compressor/interface/archivejob.cpp \
    ../../deepin-compressor/interface/cliinterface.cpp \
    ../../deepin-compressor/interface/archiveentry.cpp \
    ../../deepin-compressor/interface/kprocess.cpp \
    ../../deepin-compressor/interface/archiveinterface.cpp \
    ../../deepin-compressor/interface/archiveformat.cpp \
    ../../deepin-compressor/interface/kpluginfactory.cpp \
    ../../deepin-compressor/interface/cliproperties.cpp \
    ../../deepin-compressor/interface/kpluginloader.cpp \
    ../../deepin-compressor/interface/plugin.cpp \
    ../../deepin-compressor/interface/analysepsdtool.cpp \
    ../../deepin-compressor/interface/filewatcher.cpp \
    ../../deepin-compressor/interface/structs.cpp

HEADERS += \
        cliplugin.h \
        cliplugin_global.h \
    ../../deepin-compressor/interface/cliinterface.h \
    ../../deepin-compressor/interface/kcoreaddons_export.h \
    ../../deepin-compressor/interface/jobs.h \
    ../../deepin-compressor/interface/archiveformat.h \
    ../../deepin-compressor/interface/archivejob.h \
    ../../deepin-compressor/interface/kprocess_p.h \
    ../../deepin-compressor/interface/mimetypes.h \
    ../../deepin-compressor/interface/pluginmanager.h \
    ../../deepin-compressor/interface/archive_manager.h \
    ../../deepin-compressor/interface/kpluginmetadata.h \
    ../../deepin-compressor/interface/archivejob_p.h \
    ../../deepin-compressor/interface/archiveinterface.h \
    ../../deepin-compressor/interface/queries.h \
    ../../deepin-compressor/interface/kpluginfactory.h \
    ../../deepin-compressor/interface/kpluginloader.h \
    ../../deepin-compressor/interface/desktopfileparser_p.h \
    ../../deepin-compressor/interface/cliproperties.h \
    ../../deepin-compressor/interface/kpluginfactory_p.h \
    ../../deepin-compressor/interface/options.h \
    ../../deepin-compressor/interface/archiveentry.h \
    ../../deepin-compressor/interface/kprocess.h \
    ../../deepin-compressor/interface/kexportplugin.h \
    ../../deepin-compressor/interface/plugin.h \
    ../../deepin-compressor/interface/analysepsdtool.h \
    ../../deepin-compressor/interface/filewatcher.h \
    ../../deepin-compressor/interface/structs.h

unix {
    target.path = /usr/lib/deepin-compressor/plugins
    INSTALLS += target
}

#EXAMPLE_FILES = kerfuffle_cli7z.json

DISTFILES += \
    kerfuffle_cli7z.json

