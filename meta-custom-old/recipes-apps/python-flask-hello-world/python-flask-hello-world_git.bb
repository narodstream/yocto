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

SRC_URI = "git://github.com/narodstream/yocto.git;protocol=https \
file://flask-hello-world.service \
file://0001-change-port-to-7777.patch \
"

# Modify these as desired
PV = "1.00+git${SRCPV}"
SRCREV = "15896d7a7535c334e6cde22742a03f3936b776d4"

S = "${WORKDIR}/git"

inherit systemd

RDEPENDS_${PN} = "python3 python3-flask"

do_install () {
	# Install the python script
	install -m 0755 -d "${D}${bindir}"
	install -m 0755 app.py "${D}${bindir}"

	# Install service
	install -m 0755 -d "${D}${systemd_unitdir}/system/"
	install -m 0644 "${WORKDIR}/flask-hello-world.service" "${D}${systemd_unitdir}/system/"
}

FILES_${PN} += " \
  ${bindir}/app.py \
  ${systemd_unitdir}/system/flask-hello-world.service \
"

SYSTEMD_SERVICE_${PN} = "flask-hello-world.service"
