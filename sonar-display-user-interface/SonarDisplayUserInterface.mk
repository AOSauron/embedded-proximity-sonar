SONARDISPLAYUSERINTERFACE_VERSION = 1.0
SONARDISPLAYUSERINTERFACE_SITE = $(TOPDIR)/package/SonarDisplayUserInterface
SONARDISPLAYUSERINTERFACE_SITE_METHOD = local
SONARDISPLAYUSERINTERFACE_LICENSE = GPLv3+

define SONARDISPLAYUSERINTERFACE_BUILD_CMDS
	CC=$(TARGET_CC) $(MAKE) -C $(@D)
endef

define SONARDISPLAYUSERINTERFACE_INSTALL_TARGET_CMDS
	prefix=$(TARGET_DIR) $(MAKE) -C $(@D) install
endef

$(eval $(generic-package))