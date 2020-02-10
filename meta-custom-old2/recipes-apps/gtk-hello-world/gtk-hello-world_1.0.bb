SUMMARY = "Hello World GTK App"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://COPYING;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS = "libconfig pkgconfig-native gtk+ gtk+3 glib-2.0 libxml2 gnome-common glade"

SRC_URI = " \
file://COPYING \
file://gtk-hello-world.c \
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
