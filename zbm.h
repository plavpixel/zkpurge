#ifndef ZBM_H
#define ZBM_H

#include "defs.h"

struct kernel_list zbm_scan_kernels(const char* path);
void zbm_free_list(struct kernel_list list);
void zbm_purge_version(const char* path, const char* version);

#endif