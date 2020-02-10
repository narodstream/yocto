SUMMARY = "Hello World CPP App"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://COPYING;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
file://COPYING \
file://cpp-hello-world.cpp \
file://Makefile \
"

S = "${WORKDIR}"

do_configure () {
	:
}

do_compile() {
	oe_runmake
}

do_install () {
	oe_runmake install 'DESTDIR=${D}'
}
