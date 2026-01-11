#ifndef UTIL_H
#define UTIL_H

bool is_root(void);
const char* detect_zbm_path(void);
int natural_compare(const char* s1, const char* s2);
int entry_cmp(const void* a, const void* b);
char* get_symlink_target_version(const char* dir, const char* link_name);

#endif