################################################################################
#
# workQueue
#
################################################################################

WORK_VERSION = 1.0
WORK_METHOD = local
WORK_SITE = /group/SYSO_WS1415_9/syso_ws1415_9/v5/module-src/work

define WORK_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define WORK_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)/lib/modules/3.17.2/kernel/driver/work
	$(INSTALL) -m 0755 -D $(@D)/* $(TARGET_DIR)/lib/modules/3.17.2/kernel/driver/work
endef

#define WORK_CLEAN_CMDS
#	$(MAKE) -C $(GPIO_SITE) clean
#endef
#
#define WORK_UNINSTALL_TARGET_CMDS
#	rm -f $(TARGET_DIR)/usr/gpio
#endef

$(eval $(generic-package))
