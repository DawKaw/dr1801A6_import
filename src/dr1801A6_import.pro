QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32:RC_ICONS += Icon1.ico

VERSION_MAJOR = 1
VERSION_MINOR = 0
VERSION_BUILD = 1
win32:VERSION_BUILD2 = 0

win32:DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
       "VERSION_MINOR=$$VERSION_MINOR"\
       "VERSION_BUILD=$$VERSION_BUILD"\
       "VERSION_BUILD2=$$VERSION_BUILD2"
else:DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
       "VERSION_MINOR=$$VERSION_MINOR"\
       "VERSION_BUILD=$$VERSION_BUILD"

QMAKE_TARGET_COMPANY = Private
QMAKE_TARGET_PRODUCT = "DR-1801UV import export"
QMAKE_TARGET_DESCRIPTION = Contact data import, export
QMAKE_TARGET_COPYRIGHT = SP6ADD
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialogAbout.cpp \
    dr1801a6i.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    dialogAbout.h \
    dr1801a6i.h \
    mainwindow.h

FORMS += \
    dialogAbout.ui \
    mainwindow.ui

# Create language files on Linux os
# lupdate -verbose -noobsolete dr1801A6_import.pro
# linguist lang/dr1801A6_import_pl_PL.ts
# linguist lang/dr1801A6_import_de_DE.ts
# lrelease dr1801A6_import.pro

# Create language files on Win os
# cd C:\Users\d.kowalczyk\Documents\QT\dr1801A6_import

# C:\Qt\Qt5.12.10\5.12.10\mingw73_32\bin\lupdate.exe -verbose -noobsolete dr1801A6_import.pro
# C:\Qt\Qt5.12.10\5.12.10\mingw73_32\bin\linguist.exe dr1801A6_import_pl_PL.ts
# C:\Qt\Qt5.12.10\5.12.10\mingw73_32\bin\linguist.exe dr1801A6_import_de_DE.ts
# C:\Qt\Qt5.12.10\5.12.10\mingw73_32\bin\lrelease.exe dr1801A6_import.pro



TRANSLATIONS += \
    dr1801A6_import_pl_PL.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    resources.qrc
