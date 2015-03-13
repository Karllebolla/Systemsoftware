################################################################################
#
# ledflip
#
################################################################################

LEDFLIP_VERSION = 1.0
LEDFLIP_SITE_METHOD = local
LEDFLIP_SITE = /group/SYSO_WS1415_9/syso_ws1415_9/v6/module-src/ledflip

define LEDFLIP_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define LEDFLIP_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)/usr/ledflip/
	$(INSTALL) -m 0755 -D $(@D)/* $(TARGET_DIR)/usr/ledflip
endef

#define LEDFLIP_CLEAN_CMDS
#	$(MAKE) -C $(GPIO_SITE) clean
#endef
#
#define LEDFLIP_UNINSTALL_TARGET_CMDS
#	rm -f $(TARGET_DIR)/usr/gpio
#endef

$(eval $(generic-package))
