################################################################################
#
# buffer
#
################################################################################

BUFFER_VERSION = 1.0
BUFFER_SITE_METHOD = local
BUFFER_SITE = /group/SYSO_WS1415_9/syso_ws1415_9/v5/module-src/buffer

define BUFFER_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define BUFFER_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)/lib/modules/3.17.2/kernel/driver/buffer
	$(INSTALL) -m 0755 -D $(@D)/myTSBuf.ko $(TARGET_DIR)/lib/modules/3.17.2/kernel/driver/buffer
endef

#define BUFFER_CLEAN_CMDS
#	$(MAKE) -C $(GPIO_SITE) clean
#endef
#
#define BUFFER_UNINSTALL_TARGET_CMDS
#	rm -f $(TARGET_DIR)/usr/gpio
#endef

$(eval $(generic-package))
