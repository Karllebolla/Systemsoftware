################################################################################
#
# gpio
#
################################################################################

GPIO_VERSION = 1.0
GPIO_SITE_METHOD = local
GPIO_SITE = /group/SYSO_WS1415_9/syso_ws1415_9/v3/Application/gpio

define GPIO_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" LD="$(TAGET_LD)" -C $(@D)
endef

define GPIO_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 -D $(@D)/* $(TARGET_DIR)/usr
endef

#define GPIO_CLEAN_CMDS
#	$(MAKE) -C $(GPIO_SITE) clean
#endef
#
#define GPIO_UNINSTALL_TARGET_CMDS
#	rm -f $(TARGET_DIR)/usr/gpio
#endef

$(eval $(generic-package))
