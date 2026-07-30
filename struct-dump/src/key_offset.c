// SPDX-License-Identifier: GPL-2.0
#include <linux/compiler.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <net/net_namespace.h>
#include <linux/netdevice.h>
#include <linux/stddef.h>

#define KEY_OFFSET_PREFIX "-> "

#define EMIT_LINE(body) \
	asm volatile("\n.ascii \"" KEY_OFFSET_PREFIX body "\"")

#define EMIT_LINE_VAL(body, val) \
	asm volatile("\n.ascii \"" KEY_OFFSET_PREFIX body "\"" : : "i" (val))

#define BEGIN_STRUCT(type) \
	EMIT_LINE(#type ": {");
#define END_STRUCT(type) \
	EMIT_LINE_VAL("  $$:%0", sizeof(struct type)); \
	EMIT_LINE("},");
#define STRUCT_MEMBER(type, member) \
	EMIT_LINE_VAL("  " #member ":%0,", offsetof(struct type, member));

int main(void)
{
	BEGIN_STRUCT(module);
	STRUCT_MEMBER(module, mkobj)
	STRUCT_MEMBER(module, version)
	STRUCT_MEMBER(module, syms)
	STRUCT_MEMBER(module, gpl_syms)
	STRUCT_MEMBER(module, gpl_crcs)
	STRUCT_MEMBER(module, init)
	STRUCT_MEMBER(module, arch)
	STRUCT_MEMBER(module, taints)
#ifdef CONFIG_GENERIC_BUG
	STRUCT_MEMBER(module, num_bugs)
#endif
	STRUCT_MEMBER(module, args)
#ifdef CONFIG_JUMP_LABEL
	STRUCT_MEMBER(module, jump_entries)
#endif
#ifdef CONFIG_MODULE_UNLOAD
	STRUCT_MEMBER(module, source_list)
#endif
#ifdef CONFIG_KPROBES
	STRUCT_MEMBER(module, kprobes_text_start)
#endif
#ifdef CONFIG_DEBUG_INFO_BTF_MODULES
	STRUCT_MEMBER(module, btf_data_size)
#endif
	END_STRUCT(module);

	BEGIN_STRUCT(module_memory);
	END_STRUCT(module_memory);

	BEGIN_STRUCT(file_operations);
	STRUCT_MEMBER(file_operations, owner);
	STRUCT_MEMBER(file_operations, llseek);
	STRUCT_MEMBER(file_operations, unlocked_ioctl);
	END_STRUCT(file_operations);

	BEGIN_STRUCT(task_struct);
	STRUCT_MEMBER(task_struct, __state);
	STRUCT_MEMBER(task_struct, flags);
	STRUCT_MEMBER(task_struct, mm);
	STRUCT_MEMBER(task_struct, personality);
	STRUCT_MEMBER(task_struct, atomic_flags);
	STRUCT_MEMBER(task_struct, nsproxy);
	STRUCT_MEMBER(task_struct, seccomp);
	//STRUCT_MEMBER(task_struct, rseq);
	STRUCT_MEMBER(task_struct, rcu);
	STRUCT_MEMBER(task_struct, bpf_net_context);
	STRUCT_MEMBER(task_struct, kstack_offset);
	STRUCT_MEMBER(task_struct, thread);
	END_STRUCT(task_struct);

	BEGIN_STRUCT(net);
	STRUCT_MEMBER(net, gen);
	STRUCT_MEMBER(net, net_cookie);
	STRUCT_MEMBER(net, diag_nlsk);
#ifdef CONFIG_XDP_SOCKETS
	STRUCT_MEMBER(net, xdp);
#endif
#if IS_ENABLED(CONFIG_VSOCKETS)
	//STRUCT_MEMBER(net, vsock);
#endif
	STRUCT_MEMBER(net, diag_nlsk);
	END_STRUCT(net);

	BEGIN_STRUCT(net_device_ops);
	STRUCT_MEMBER(net_device_ops, ndo_vlan_rx_kill_vid);
	STRUCT_MEMBER(net_device_ops, ndo_set_vf_mac);
	STRUCT_MEMBER(net_device_ops, ndo_add_slave);
	STRUCT_MEMBER(net_device_ops, ndo_bpf);
	STRUCT_MEMBER(net_device_ops, ndo_hwtstamp_set);
	END_STRUCT(net_device_ops);

	BEGIN_STRUCT(net_device);
	STRUCT_MEMBER(net_device, name);
	STRUCT_MEMBER(net_device, operstate);
	STRUCT_MEMBER(net_device, dev_addr);
	STRUCT_MEMBER(net_device, broadcast);
	STRUCT_MEMBER(net_device, watchdog_timer);
	STRUCT_MEMBER(net_device, priv_destructor);
	STRUCT_MEMBER(net_device, dev);
	STRUCT_MEMBER(net_device, threaded);
	STRUCT_MEMBER(net_device, net_notifier_list);
	STRUCT_MEMBER(net_device, devlink_port);
	STRUCT_MEMBER(net_device, irq_moder);
	STRUCT_MEMBER(net_device, priv);
	END_STRUCT(net_device);
	return 0;
}
