################################################################################
#
# mygpio
#
################################################################################

MYGPIO_VERSION = 1.0
MYGPIO_SITE_METHOD = local
MYGPIO_SITE = /group/SYSO_WS1415_9/syso_ws1415_9/v6/module-src/mygpio

define MYGPIO_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define MYGPIO_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)/lib/modules/3.17.2/kernel/driver/mygpio
	$(INSTALL) -m 0755 -D $(@D)/* $(TARGET_DIR)/lib/modules/3.17.2/kernel/driver/mygpio
endef

#define MYGPIO_CLEAN_CMDS
#	$(MAKE) -C $(GPIO_SITE) clean
#endef
#
#define MYGPIO_UNINSTALL_TARGET_CMDS
#	rm -f $(TARGET_DIR)/usr/gpio
#endef

$(eval $(generic-package))
