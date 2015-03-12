################################################################################
#
# timer
#
################################################################################

TIMER_VERSION = 1.0
TIMER_SITE_METHOD = local
TIMER_SITE = /group/SYSO_WS1415_9/syso_ws1415_9/v5/module-src/timer

define TIMER_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define TIMER_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)/lib/modules/3.17.2/kernel/driver/timer
	$(INSTALL) -m 0755 -D $(@D)/* $(TARGET_DIR)/lib/modules/3.17.2/kernel/driver/timer
endef

#define TIMER_CLEAN_CMDS
#	$(MAKE) -C $(GPIO_SITE) clean
#endef
#
#define TIMER_UNINSTALL_TARGET_CMDS
#	rm -f $(TARGET_DIR)/usr/gpio
#endef

$(eval $(generic-package))
