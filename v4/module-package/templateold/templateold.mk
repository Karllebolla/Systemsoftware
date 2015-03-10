################################################################################
#
# templateold
#
################################################################################

TEMPLATEOLD_VERSION = 1.0
TEMPLATEOLD_SITE_METHOD = local
TEMPLATEOLD_SITE = /group/SYSO_WS1415_9/syso_ws1415_9/v4/module-src/templateold

define TEMPLATEOLD_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define TEMPLATEOLD_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)/lib/modules/3.17.2/kernel/driver/templateold
	$(INSTALL) -m 0755 -D $(@D)/* $(TARGET_DIR)/lib/modules/3.17.2/kernel/driver/templateold
endef

#define TEMPLATE_CLEAN_CMDS
#	$(MAKE) -C $(GPIO_SITE) clean
#endef
#
#define TEMPLATE_UNINSTALL_TARGET_CMDS
#	rm -f $(TARGET_DIR)/usr/gpio
#endef

$(eval $(generic-package))
