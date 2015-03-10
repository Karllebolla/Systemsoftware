################################################################################
#
# template
#
################################################################################

TEMPLATE_VERSION = 1.0
TEMPLATE_SITE_METHOD = local
TEMPLATE_SITE = /group/SYSO_WS1415_9/syso_ws1415_9/v4/module-src/template

define TEMPLATE_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define TEMPLATE_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)/lib/modules/3.17.2/kernel/driver/template
	$(INSTALL) -m 0755 -D $(@D)/* $(TARGET_DIR)/lib/modules/3.17.2/kernel/driver/template
endef

#define TEMPLATE_CLEAN_CMDS
#	$(MAKE) -C $(GPIO_SITE) clean
#endef
#
#define TEMPLATE_UNINSTALL_TARGET_CMDS
#	rm -f $(TARGET_DIR)/usr/gpio
#endef

$(eval $(generic-package))
