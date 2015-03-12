################################################################################
#
# access
#
################################################################################

ACCESS_VERSION = 1.0
ACCESS_SITE_METHOD = local
ACCESS_SITE = /group/SYSO_WS1415_9/syso_ws1415_9/v5/module-src/access

define ACCESS_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define ACCESS_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)/root/template
	$(INSTALL) -m 0755 -D $(@D)/* $(TARGET_DIR)/root/template
endef

#define ACCESS_CLEAN_CMDS
#	$(MAKE) -C $(GPIO_SITE) clean
#endef
#
#define ACCESS_UNINSTALL_TARGET_CMDS
#	rm -f $(TARGET_DIR)/usr/gpio
#endef

$(eval $(generic-package))
