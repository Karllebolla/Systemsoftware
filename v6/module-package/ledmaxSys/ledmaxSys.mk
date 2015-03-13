################################################################################
#
# ledmaxSys
#
################################################################################

LEDMAXSYS_VERSION = 1.0
LEDMAXSYS_SITE_METHOD = local
LEDMAXSYS_SITE = /group/SYSO_WS1415_9/syso_ws1415_9/v6/module-src/ledmaxSys

define LEDMAXSYS_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define LEDMAXSYS_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)/usr/ledmaxSys/
	$(INSTALL) -m 0755 -D $(@D)/* $(TARGET_DIR)/usr/ledmaxSys
endef

#define LEDMAXSYS_CLEAN_CMDS
#	$(MAKE) -C $(GPIO_SITE) clean
#endef
#
#define LEDMAXSYS_UNINSTALL_TARGET_CMDS
#	rm -f $(TARGET_DIR)/usr/gpio
#endef

$(eval $(generic-package))
