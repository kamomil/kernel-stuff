################################################################################
#
## libfoo
#
#################################################################################
MYPACK_VERSION = 1.0
MYPACK_SITE = ~/linux-kernel-module-cheat4/buildroot/package/mypack
MYPACK_SITE_METHOD = local

define MYPACK_BUILD_CMDS
	$(TARGET_MAKE_ENV) $(MAKE) CC="$(TARGET_CC)"
endef

define MYPACK_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/divide_err $(TARGET_DIR)/usr/bin/
endef


