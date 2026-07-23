

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=struct-dump
PKG_RELEASE:=1
PKG_TARGETS:=bin

include $(INCLUDE_DIR)/package.mk

define KernelPackage/struct-dump
  SUBMENU:=Misc
  TITLE:=Dump kernel struct offset
  KCONFIG:=
endef

STRUCT_INCLUDE:=\
	linux/module.h \
	linux/fs.h \
	linux/sched.h \
	net/net_namespace.h \
	linux/netdevice.h

STRUCT_NAME:=\
	module \
	file_operations \
	task_struct \
	net \
	net_device_ops

define Build/Compile
	$(KERNEL_MAKE) M="$(PKG_BUILD_DIR)" \
		CONFIG_DUMPSTRUCT_KEY_OFFSET=y \
		STRUCT_INCLUDE="$(STRUCT_INCLUDE)" \
		STRUCT_NAME="$(STRUCT_NAME)"
endef

define Build/InstallDev
	$(INSTALL_DIR) $(KERNEL_BUILD_DIR)/debug/struct-dump
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/*.txt $(KERNEL_BUILD_DIR)/debug/struct-dump/
endef

$(eval $(call KernelPackage,struct-dump))
