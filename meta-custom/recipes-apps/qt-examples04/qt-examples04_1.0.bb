SUMMARY = "Qt Examples04 Application Recipe"
DESCRIPTION = "This recipe builds a Qt project for a Examples04 application."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://COPYING;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS += "qtbase"

SRC_URI = " \
file://* \
"
S = "${WORKDIR}"

do_install_append() {
    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/build/Examples04 ${D}${bindir}/
}

inherit qmake5
