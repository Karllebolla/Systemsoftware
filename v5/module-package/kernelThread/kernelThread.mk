################################################################################
#
# kernelThread
#
################################################################################

KERNELTHREAD_VERSION = 1.0
KERNELTHREAD_SITE_METHOD = local
KERNELTHREAD_SITE = /group/SYSO_WS1415_9/syso_ws1415_9/v5/module-src/kernelThread

define KERNELTHREAD_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define KERNELTHREAD_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)/lib/modules/3.17.2/kernel/driver/kernelThread
	$(INSTALL) -m 0755 -D $(@D)/* $(TARGET_DIR)/lib/modules/3.17.2/kernel/driver/kernelThread
endef

#define KERNELTHREAD_CLEAN_CMDS
#	$(MAKE) -C $(GPIO_SITE) clean
#endef
#
#define KERNELTHREAD_UNINSTALL_TARGET_CMDS
#	rm -f $(TARGET_DIR)/usr/gpio
#endef

$(eval $(generic-package))
