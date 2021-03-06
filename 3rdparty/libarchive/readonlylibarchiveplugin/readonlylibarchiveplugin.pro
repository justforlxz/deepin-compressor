#-------------------------------------------------
#
# Project created by QtCreator 2019-08-27T14:48:47
#
#-------------------------------------------------

QT       -= gui
QT += dtkwidget KCodecs
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = readonlylibarchiveplugin
TEMPLATE = lib
LIBS +=  -larchive
DEFINES += READONLYLIBARCHIVEPLUGIN_LIBRARY
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
INCLUDEPATH += $$PWD/../../../deepin-compressor/source/inc/ \
                $$PWD/../../../deepin-compressor/interface/ \
               $$PWD/../../ChardetDetector/

SOURCES += \
    ../../common/common.cpp \
        readonlylibarchiveplugin.cpp \
    ../libarchiveplugin.cpp \
    ../../../deepin-compressor/interface/options.cpp \
    ../../../deepin-compressor/interface/archiveentry.cpp \
    ../../../deepin-compressor/interface/desktopfileparser.cpp \
    ../../../deepin-compressor/interface/cliinterface.cpp \
    ../../../deepin-compressor/interface/kprocess.cpp \
    ../../../deepin-compressor/interface/archivejob.cpp \
    ../../../deepin-compressor/interface/kpluginmetadata.cpp \
    ../../../deepin-compressor/interface/jobs.cpp \
    ../../../deepin-compressor/interface/queries.cpp \
    ../../../deepin-compressor/interface/archive_manager.cpp \
    ../../../deepin-compressor/interface/cliproperties.cpp \
    ../../../deepin-compressor/interface/archiveformat.cpp \
    ../../../deepin-compressor/interface/mimetypes.cpp \
    ../../../deepin-compressor/interface/archiveinterface.cpp \
    ../../../deepin-compressor/interface/kpluginfactory.cpp \
    ../../../deepin-compressor/interface/pluginmanager.cpp \
    ../../../deepin-compressor/interface/plugin.cpp \
    ../../../deepin-compressor/interface/kpluginloader.cpp \
    ../../../deepin-compressor/interface/analysepsdtool.cpp \
    ../../../deepin-compressor/interface/filewatcher.cpp \
    ../../common/detectencoding.cpp \
    ../../../deepin-compressor/interface/structs.cpp \
    ../../ChardetDetector/tables/LangBulgarianModel.cpp \
    ../../ChardetDetector/tables/LangCyrillicModel.cpp \
    ../../ChardetDetector/tables/LangGreekModel.cpp \
    ../../ChardetDetector/tables/LangHebrewModel.cpp \
    ../../ChardetDetector/tables/LangHungarianModel.cpp \
    ../../ChardetDetector/tables/LangThaiModel.cpp \
    ../../ChardetDetector/chardet.cpp \
    ../../ChardetDetector/CharDistribution.cpp \
    ../../ChardetDetector/JpCntx.cpp \
    ../../ChardetDetector/nsBig5Prober.cpp \
    ../../ChardetDetector/nsCharSetProber.cpp \
    ../../ChardetDetector/nsEscCharsetProber.cpp \
    ../../ChardetDetector/nsEscSM.cpp \
    ../../ChardetDetector/nsEUCJPProber.cpp \
    ../../ChardetDetector/nsEUCKRProber.cpp \
    ../../ChardetDetector/nsEUCTWProber.cpp \
    ../../ChardetDetector/nsGB2312Prober.cpp \
    ../../ChardetDetector/nsHebrewProber.cpp \
    ../../ChardetDetector/nsLatin1Prober.cpp \
    ../../ChardetDetector/nsMBCSGroupProber.cpp \
    ../../ChardetDetector/nsMBCSSM.cpp \
    ../../ChardetDetector/nsSBCharSetProber.cpp \
    ../../ChardetDetector/nsSBCSGroupProber.cpp \
    ../../ChardetDetector/nsSJISProber.cpp \
    ../../ChardetDetector/nsUniversalDetector.cpp \
    ../../ChardetDetector/nsUTF8Prober.cpp

HEADERS += \
        ../../common/common.h \
        readonlylibarchiveplugin.h \
        readonlylibarchiveplugin_global.h \ 
    ../libarchiveplugin.h \
    ../../../deepin-compressor/interface/archiveformat.h \
    ../../../deepin-compressor/interface/kpluginmetadata.h \
    ../../../deepin-compressor/interface/kcoreaddons_export.h \
    ../../../deepin-compressor/interface/kprocess.h \
    ../../../deepin-compressor/interface/plugin.h \
    ../../../deepin-compressor/interface/jobs.h \
    ../../../deepin-compressor/interface/kexportplugin.h \
    ../../../deepin-compressor/interface/queries.h \
    ../../../deepin-compressor/interface/kpluginfactory.h \
    ../../../deepin-compressor/interface/archivejob.h \
    ../../../deepin-compressor/interface/cliinterface.h \
    ../../../deepin-compressor/interface/mimetypes.h \
    ../../../deepin-compressor/interface/kpluginfactory_p.h \
    ../../../deepin-compressor/interface/archive_manager.h \
    ../../../deepin-compressor/interface/kpluginloader.h \
    ../../../deepin-compressor/interface/desktopfileparser_p.h \
    ../../../deepin-compressor/interface/options.h \
    ../../../deepin-compressor/interface/archiveentry.h \
    ../../../deepin-compressor/interface/archiveinterface.h \
    ../../../deepin-compressor/interface/cliproperties.h \
    ../../../deepin-compressor/interface/kprocess_p.h \
    ../../../deepin-compressor/interface/archivejob_p.h \
    ../../../deepin-compressor/interface/pluginmanager.h \
    ../../../deepin-compressor/interface/analysepsdtool.h \
    ../../../deepin-compressor/interface/filewatcher.h \
    ../../common/detectencoding.h \
    ../../../deepin-compressor/interface/structs.h \
    ../../ChardetDetector/chardet.h \
    ../../ChardetDetector/CharDistribution.h \
    ../../ChardetDetector/JpCntx.h \
    ../../ChardetDetector/nsBig5Prober.h \
    ../../ChardetDetector/nsCharSetProber.h \
    ../../ChardetDetector/nsCodingStateMachine.h \
    ../../ChardetDetector/nscore.h \
    ../../ChardetDetector/nsEscCharsetProber.h \
    ../../ChardetDetector/nsEUCJPProber.h \
    ../../ChardetDetector/nsEUCKRProber.h \
    ../../ChardetDetector/nsEUCTWProber.h \
    ../../ChardetDetector/nsGB2312Prober.h \
    ../../ChardetDetector/nsHebrewProber.h \
    ../../ChardetDetector/nsLatin1Prober.h \
    ../../ChardetDetector/nsMBCSGroupProber.h \
    ../../ChardetDetector/nsPkgInt.h \
    ../../ChardetDetector/nsSBCharSetProber.h \
    ../../ChardetDetector/nsSBCSGroupProber.h \
    ../../ChardetDetector/nsSJISProber.h \
    ../../ChardetDetector/nsUniversalDetector.h \
    ../../ChardetDetector/nsUTF8Prober.h \
    ../../ChardetDetector/prmem.h \
    ../../ChardetDetector/version.h

unix {
    target.path = /usr/lib/deepin-compressor/plugins
    INSTALLS += target
}

DISTFILES += \
    kerfuffle_libarchive_readonly.json \
    ../../ChardetDetector/tables/Big5Freq.tab \
    ../../ChardetDetector/tables/EUCKRFreq.tab \
    ../../ChardetDetector/tables/EUCTWFreq.tab \
    ../../ChardetDetector/tables/GB2312Freq.tab \
    ../../ChardetDetector/tables/JISFreq.tab \
    ../../ChardetDetector/Big5Freq.tab \
    ../../ChardetDetector/EUCKRFreq.tab \
    ../../ChardetDetector/EUCTWFreq.tab \
    ../../ChardetDetector/GB2312Freq.tab \
    ../../ChardetDetector/JISFreq.tab

SUBDIRS += \
    ../../ChardetDetector/ChardetDetector.pro




