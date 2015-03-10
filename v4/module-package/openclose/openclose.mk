################################################################################
#
# openclose
#
################################################################################

OPENCLOSE_VERSION = 1.0
OPENCLOSE_SITE_METHOD = local
OPENCLOSE_SITE = /group/SYSO_WS1415_9/syso_ws1415_9/v4/module-src/openclose

define OPENCLOSE_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define OPENCLOSE_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)/lib/modules/3.17.2/kernel/driver/openclose
	$(INSTALL) -m 0755 -D $(@D)/* $(TARGET_DIR)/lib/modules/3.17.2/kernel/driver/openclose
endef

#define OPENCLOSE_CLEAN_CMDS
#	$(MAKE) -C $(GPIO_SITE) clean
#endef
#
#define OPENCLOSE_UNINSTALL_TARGET_CMDS
#	rm -f $(TARGET_DIR)/usr/gpio
#endef

$(eval $(generic-package))
