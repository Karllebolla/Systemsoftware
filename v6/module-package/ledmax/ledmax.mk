################################################################################
#
# ledmax
#
################################################################################

LEDMAX_VERSION = 1.0
LEDMAX_SITE_METHOD = local
LEDMAX_SITE = /group/SYSO_WS1415_9/syso_ws1415_9/v6/module-src/ledmax

define LEDMAX_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define LEDMAX_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)/usr/ledmax/
	$(INSTALL) -m 0755 -D $(@D)/* $(TARGET_DIR)/usr/ledmax
endef

#define LEDMAX_CLEAN_CMDS
#	$(MAKE) -C $(GPIO_SITE) clean
#endef
#
#define LEDMAX_UNINSTALL_TARGET_CMDS
#	rm -f $(TARGET_DIR)/usr/gpio
#endef

$(eval $(generic-package))
