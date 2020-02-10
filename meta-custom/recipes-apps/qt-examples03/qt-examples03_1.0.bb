SUMMARY = "Qt Examples03 Application Recipe"
DESCRIPTION = "This recipe builds a Qt project for a prog01 application."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://COPYING;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS += "qtbase"

SRC_URI = " \
file://COPYING \
file://Examples03.pro \
file://main.cpp \
file://mainwindow.cpp \
file://mainwindow.h \
file://mainwindow.ui \
"
S = "${WORKDIR}"

do_install_append() {
    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/build/Examples03 ${D}${bindir}/
}

inherit qmake5
