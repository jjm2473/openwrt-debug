ifneq ($(KERNELRELEASE),)

STRUCT_INCLUDE ?= linux/module.h
STRUCT_NAME ?= module
empty :=
space := $(empty) $(empty)
STRUCT_OUT ?= $(addsuffix .txt,$(strip $(STRUCT_NAME)))
STRUCT_OUT_TARGETS := $(addprefix $(obj)/,$(STRUCT_OUT))
CONFIG_DUMPSTRUCT_KEY_OFFSET ?= n
CONFIG_DUMPSTRUCT_STRUCT_OUT ?= y

always-$(CONFIG_DUMPSTRUCT_KEY_OFFSET) += key_offset.txt
always-$(CONFIG_DUMPSTRUCT_STRUCT_OUT) += $(STRUCT_OUT)
targets += .key_offset.s key_offset.txt .multi.merged.struct_input.i .multi.struct_input.c .multi.struct_names .multi.struct_outs .multi.struct_out.done $(STRUCT_OUT)

SED_BIN := $(if $(shell command -v gsed 2>/dev/null),gsed,sed)

quiet_cmd_key_offset = KEYOFFSET $@
cmd_key_offset = \
	$(CC) $(KBUILD_CPPFLAGS) $(KBUILD_CFLAGS) $(LINUXINCLUDE) -S -o $(obj)/.key_offset.s $(src)/key_offset.c; \
	{ printf '{\n'; $(SED_BIN) -n 's/^.*-> \(.*\)".*$$/\1/p' $(obj)/.key_offset.s; printf '}\n'; } > $@

$(obj)/key_offset.txt: $(src)/key_offset.c FORCE
	$(call if_changed,key_offset)

quiet_cmd_module_struct = DUMPSTRUCT $(STRUCT_OUT)
cmd_module_struct = \
	: > $(obj)/.multi.struct_input.c; \
	for hdr in $(STRUCT_INCLUDE); do \
		printf '\043include <'"$$hdr"'>\n' >> $(obj)/.multi.struct_input.c; \
	done; \
	$(CC) $(KBUILD_CPPFLAGS) $(KBUILD_CFLAGS) $(LINUXINCLUDE) -E -P -x c -o $(obj)/.multi.merged.struct_input.i $(obj)/.multi.struct_input.c; \
	printf '%s\n' $(STRUCT_NAME) > $(obj)/.multi.struct_names; \
	printf '%s\n' $(STRUCT_OUT) > $(obj)/.multi.struct_outs; \
	name_cnt=`wc -l < $(obj)/.multi.struct_names`; \
	out_cnt=`wc -l < $(obj)/.multi.struct_outs`; \
	if [ "$$name_cnt" -ne "$$out_cnt" ]; then \
		echo "error: STRUCT_NAME count ($$name_cnt) != STRUCT_OUT count ($$out_cnt)" >&2; \
		exit 1; \
	fi; \
	paste $(obj)/.multi.struct_names $(obj)/.multi.struct_outs | while IFS="	" read -r s out; do \
		awk -v name="$$s" -f $(src)/extract_struct.awk $(obj)/.multi.merged.struct_input.i > $(obj)/$$out || exit $$?; \
		printf '\n' >> $(obj)/$$out; \
	done; \
	touch $@

$(obj)/.multi.struct_out.done: $(src)/extract_struct.awk FORCE
	$(call if_changed,module_struct)

$(STRUCT_OUT_TARGETS): $(obj)/.multi.struct_out.done

else

KDIR ?= $(abspath $(PWD)/../..)
PWD := $(shell pwd)
ARCH ?= arm64
HOST_CC ?= clang
CROSS_COMPILE ?= $(cross_compile)
SED_BIN ?= $(if $(shell command -v gsed 2>/dev/null),gsed,sed)
STRUCT_INCLUDE ?= linux/module.h
STRUCT_NAME ?= module
empty :=
space := $(empty) $(empty)
STRUCT_OUT ?= $(addsuffix .txt,$(strip $(STRUCT_NAME)))
CONFIG_DUMPSTRUCT_KEY_OFFSET ?= n
CONFIG_DUMPSTRUCT_STRUCT_OUT ?= y

.PHONY: all key-offset struct-out test-host clean

all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) LD=$(LD) CROSS_COMPILE=$(CROSS_COMPILE) HOSTCFLAGS="$(HOSTCFLAGS)" \
		CONFIG_DUMPSTRUCT_KEY_OFFSET=$(CONFIG_DUMPSTRUCT_KEY_OFFSET) CONFIG_DUMPSTRUCT_STRUCT_OUT=$(CONFIG_DUMPSTRUCT_STRUCT_OUT) modules

key-offset:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) LD=$(LD) CROSS_COMPILE=$(CROSS_COMPILE) HOSTCFLAGS="$(HOSTCFLAGS)" SED_BIN=$(SED_BIN) \
		CONFIG_DUMPSTRUCT_KEY_OFFSET=y CONFIG_DUMPSTRUCT_STRUCT_OUT=n modules

struct-out:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) LD=$(LD) CROSS_COMPILE=$(CROSS_COMPILE) HOSTCFLAGS="$(HOSTCFLAGS)" \
		STRUCT_INCLUDE="$(STRUCT_INCLUDE)" STRUCT_NAME="$(STRUCT_NAME)" STRUCT_OUT="$(STRUCT_OUT)" \
		CONFIG_DUMPSTRUCT_KEY_OFFSET=n CONFIG_DUMPSTRUCT_STRUCT_OUT=y modules

test-host:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) LD=$(LD) CROSS_COMPILE=$(CROSS_COMPILE) HOSTCFLAGS="$(HOSTCFLAGS)" \
		SED_BIN=$(SED_BIN) STRUCT_INCLUDE="$(STRUCT_INCLUDE)" STRUCT_NAME="$(STRUCT_NAME)" STRUCT_OUT="$(STRUCT_OUT)" \
		CONFIG_DUMPSTRUCT_KEY_OFFSET=y CONFIG_DUMPSTRUCT_STRUCT_OUT=y modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) LD=$(LD) CROSS_COMPILE=$(CROSS_COMPILE) HOSTCFLAGS="$(HOSTCFLAGS)" clean

endif
