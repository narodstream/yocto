SUMMARY = "Python script desctop"
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/BSD-3-Clause;md5=550794465ba0ec5312d6919e203a55f9"

DEPENDS += "patchelf-native"

RDEPENDS_${PN} = "python3"

SRC_URI = " \
    file://my_desk.py \
    file://img \
    "

do_configure[noexec] = "1"
do_compile[noexec] = "1"

S = "${WORKDIR}"

do_install() {
    install -d ${D}${prefix}/local/desk/
    install -d ${D}${prefix}/local/desk/img
    install -m 0755 ${WORKDIR}/my_desk.py ${D}${prefix}/local/desk/
    install -m 0644 ${WORKDIR}/img/* ${D}${prefix}/local/desk/img/
}

FILES_${PN} += "${prefix}/local/desk/"
