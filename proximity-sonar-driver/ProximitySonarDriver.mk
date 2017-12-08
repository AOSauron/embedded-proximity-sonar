PROXIMITYSONARDRIVER_VERSION = 1.0
PROXIMITYSONARDRIVER_SITE = $(TOPDIR)/package/ProximitySonarDriver
PROXIMITYSONARDRIVER_SITE_METHOD = local
PROXIMITYSONARDRIVER_LICENSE = MIT

PROXIMITYSONARDRIVER_DEPENDENCIES = linux

define PROXIMITYSONARDRIVER_BUILD_CMDS
$(MAKE) -C $(LINUX_DIR) $(LINUX_MAKE_FLAGS) M=$(@D)
endef

define PROXIMITYSONARDRIVER_INSTALL_TARGET_CMDS
$(MAKE) -C $(LINUX_DIR) $(LINUX_MAKE_FLAGS) M=$(@D) modules_install
endef

$(eval $(generic-package))
