################################################################################
#
# led250
#
################################################################################

LED250_VERSION = 1.0
LED250_SITE_METHOD = local
LED250_SITE = /group/SYSO_WS1415_9/syso_ws1415_9/v6/module-src/led250

define LED250_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define LED250_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)/usr/led250/
	$(INSTALL) -m 0755 -D $(@D)/* $(TARGET_DIR)/usr/led250
endef

#define LED250_CLEAN_CMDS
#	$(MAKE) -C $(GPIO_SITE) clean
#endef
#
#define LED250_UNINSTALL_TARGET_CMDS
#	rm -f $(TARGET_DIR)/usr/gpio
#endef

$(eval $(generic-package))
