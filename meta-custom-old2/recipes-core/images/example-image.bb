# Base this image on core-image-sato
include recipes-sato/images/core-image-sato.bb

# Only allow for machines which start with "rpi"
COMPATIBLE_MACHINE = "^rpi$"

#Be abble to generate qt5 sdk
inherit populate_sdk_qt5

LICENSE = "CLOSED"
LIC_FILES_CHKSUM = ""

# Add rsync (for qt creator deployment)
IMAGE_INSTALL += "rsync"

# User configuration
inherit extrausers
EXTRA_USERS_PARAMS = "\
usermod -p \\\$1\\\$bYVe2.v8\\\$tivpm0e493fTMcfGxVac9. root \
"
inherit populate_sdk_qt5
# Only produce the "rpi-sdimg" image format
IMAGE_FSTYPES = "rpi-sdimg"
# Remove old builds
RM_OLD_IMAGE = "1"

# install kernel modules
IMAGE_INSTALL += "kernel-modules"

# Remove the splash screen
IMAGE_FEATURES_remove = "splash"

# Print this message once do rootfs() has completed
do_rootfs_append() {
    bb.plain("An image has freshly built!")
}

# Add support for wifi and bluetooth
IMAGE_INSTALL += "linux-firmware linux-firmware-bcm43430"

# Add ssh
IMAGE_FEATURES += "ssh-server-dropbear"

# Add package management
EXTRA_IMAGE_FEATURES += "package-management"
PACKAGE_CLASSES = "package_rpm"

# Install i2c-tools
IMAGE_INSTALL += "i2c-tools"

# Install vim
IMAGE_INSTALL += "vim"

# Configure timezone
IMAGE_INSTALL += "tzdata-europe tzdata"

# Add cpp hello world app
IMAGE_INSTALL += "cpp-hello-world"

# Add gtk hello world app
IMAGE_INSTALL += "gtk-hello-world"

# Install cmake hello world 
IMAGE_INSTALL += "cmake-hello-world"

# Add our python flask hello world app
IMAGE_INSTALL += "python-flask-hello-world"

# Install qt5 cinematic experience app
IMAGE_INSTALL += "cinematicexperience"

# Install qt-simple-calc
IMAGE_INSTALL += "qt-simple-calc"

# Install qt-prog01
IMAGE_INSTALL += "qt-prog01"
