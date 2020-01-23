SUMMARY = "Qt Prog01 Application Recipe"
DESCRIPTION = "This recipe builds a Qt project for a prog01 application."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS += "qtbase"

SRC_URI = "git://github.com/narodstream/prog01;branch=master"
SRCREV = "71045ed327a56e9804662a6947b13a726f04a4f0"

S = "${WORKDIR}/git"

do_install_append() {
    install -d ${D}/opt/prog01/
    install -m 0755 prog01 ${D}/opt/prog01/
}

FILES_${PN} += "/opt/prog01/prog01"

inherit qmake5
