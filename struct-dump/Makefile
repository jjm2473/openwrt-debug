

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=struct-dump
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/struct-dump
  SUBMENU:=Misc
  TITLE:=Dump kernel struct offset
  KCONFIG:=
endef


define Build/Compile
	$(KERNEL_MAKE) M="$(PKG_BUILD_DIR)" modules
endef

$(eval $(call KernelPackage,struct-dump))
