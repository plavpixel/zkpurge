#ifndef DEFS_H
#define DEFS_H

#include <stddef.h>

#define MAX_PATH_LEN 4096
#define PROG_VERSION "1.1.0"

static const char SYS_EFI_PATH[] = "/sys/firmware/efi";
static const char DEFAULT_EFI_ZBM[] = "/boot/efi/EFI/zbm";
static const char DEFAULT_MBR_ZBM[] = "/boot/zbm";

static const char VMLINUZ_PREFIX[] = "vmlinuz-";
static const char INITRAMFS_PREFIX[] = "initramfs-";
static const char INITRAMFS_SUFFIX[] = ".img";
static const char UKI_SUFFIX_LOWER[] = ".efi";
static const char UKI_SUFFIX_UPPER[] = ".EFI";

struct kernel_entry {
    char *full_filename;
    char *version;
};

struct kernel_list {
    struct kernel_entry* entries;
    size_t count;
};

#endif