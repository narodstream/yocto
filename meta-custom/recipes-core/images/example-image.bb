# Base this image on core-image-sato
include recipes-st/images/st-image-weston.bb

# Only allow for machines which start with "rpi"
COMPATIBLE_MACHINE = "(stm32mpcommon)"

#Be abble to generate qt5 sdk
inherit populate_sdk_qt5

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = ""

# Add rsync (for qt creator deployment)
IMAGE_INSTALL += "rsync"
