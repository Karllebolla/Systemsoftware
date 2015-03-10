################################################################################
#
# zero
#
################################################################################

ZERO_VERSION = 1.0
ZERO_SITE_METHOD = local
ZERO_SITE = /group/SYSO_WS1415_9/syso_ws1415_9/v4/module-src/zero

define ZERO_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define ZERO_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)/lib/modules/3.17.2/kernel/driver/zero
	$(INSTALL) -m 0755 -D $(@D)/* $(TARGET_DIR)/lib/modules/3.17.2/kernel/driver/zero
endef

#define ZERO_CLEAN_CMDS
#	$(MAKE) -C $(GPIO_SITE) clean
#endef
#
#define ZERO_UNINSTALL_TARGET_CMDS
#	rm -f $(TARGET_DIR)/usr/gpio
#endef

$(eval $(generic-package))
