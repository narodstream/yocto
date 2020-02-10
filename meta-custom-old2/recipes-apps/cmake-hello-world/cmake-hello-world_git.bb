# Recipe created by recipetool
# This is the basis of a recipe and may need further editing in order to be fully functional.
# (Feel free to remove these comments when editing.)

# Unable to find any files that looked like license statements. Check the accompanying
# documentation and source headers and set LICENSE and LIC_FILES_CHKSUM accordingly.
#
# NOTE: LICENSE is being set to "CLOSED" to allow you to at least start building - if
# this is not accurate with respect to the licensing of the software being built (it
# will not be in most cases) you must specify the correct value before using this
# recipe for anything other than initial testing/development!
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = ""

SRC_URI = "git://github.com/narodstream/cmake-hello-world.git;protocol=https"

# Modify these as desired
PV = "1.00+git${SRCPV}"
SRCREV = "19be7dd8dec1201feb3b392c99227ceec5c544c9"

S = "${WORKDIR}/git"

inherit cmake

# Specify any options you want to pass to cmake using EXTRA_OECMAKE:
EXTRA_OECMAKE = ""

do_install() {
    # Install the binary
    install -m 0755 -d "${D}${bindir}"
    install -m 0755 cmake-hello-world-cpp "${D}${bindir}"
}
