#include <linux/stddef.h>
#include <linux/module.h>

#define PRINT_SIZE(A) char (*__sizeof_##A)[sizeof(char[sizeof(struct A)])] = 1;
#define PRINT_OFFSET(A, B) char (*__offset_of_##B)[sizeof(char[offsetof(struct A, B)])] = 1;

PRINT_SIZE(module_memory) //16//72

PRINT_OFFSET(module, mkobj) //80//80
PRINT_OFFSET(module, version) //184
PRINT_OFFSET(module, syms) //208
PRINT_OFFSET(module, gpl_syms) //280
PRINT_OFFSET(module, gpl_crcs) //288
PRINT_OFFSET(module, init) //312
PRINT_OFFSET(module, arch) //432//824
PRINT_OFFSET(module, taints) //464//824
#ifdef CONFIG_GENERIC_BUG
PRINT_OFFSET(module, num_bugs) //472//832
#endif
PRINT_OFFSET(module, args) //504//864
#ifdef CONFIG_JUMP_LABEL
PRINT_OFFSET(module, jump_entries)//552
#endif
#ifdef CONFIG_MODULE_UNLOAD
PRINT_OFFSET(module, source_list)//568//928
#endif
#ifdef CONFIG_KPROBES
PRINT_OFFSET(module, kprobes_text_start)
#endif
#ifdef CONFIG_DEBUG_INFO_BTF_MODULES
PRINT_OFFSET(module, btf_data_size)
#endif

#error "DUMP end"
