#define _POSIX_C_SOURCE 200809L
#include "util.h"
#include "defs.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <ctype.h>

bool is_root(void) {
    return geteuid() == 0;
}

const char* detect_zbm_path(void) {
    struct stat st;
    if (stat(SYS_EFI_PATH, &st) == 0 && S_ISDIR(st.st_mode)) {
        return DEFAULT_EFI_ZBM;
    }
    return DEFAULT_MBR_ZBM;
}

int natural_compare(const char* s1, const char* s2) {
    while (*s1 && *s2) {
        if (isdigit((unsigned char)*s1) && isdigit((unsigned char)*s2)) {
            char *e1, *e2;
            unsigned long n1 = strtoul(s1, &e1, 10);
            unsigned long n2 = strtoul(s2, &e2, 10);

            if (n1 != n2) return (n1 < n2) ? -1 : 1;
            s1 = e1;
            s2 = e2;
        } else {
            if (*s1 != *s2) return *s1 - *s2;
            s1++;
            s2++;
        }
    }
    return *s1 - *s2;
}

int entry_cmp(const void* a, const void* b) {
    const struct kernel_entry *ka = a, *kb = b;
    return natural_compare(ka->version, kb->version);
}

char* get_symlink_target_version(const char* dir, const char* link_name) {
    char lp[MAX_PATH_LEN], tb[MAX_PATH_LEN];
    if (snprintf(lp, MAX_PATH_LEN, "%s/%s", dir, link_name) >= MAX_PATH_LEN) return NULL;

    struct stat st;
    if (lstat(lp, &st) == -1 || !S_ISLNK(st.st_mode)) return NULL;

    ssize_t len = readlink(lp, tb, MAX_PATH_LEN - 1);
    if (len == -1) return NULL;
    tb[len] = '\0';

    char* f = strrchr(tb, '/');
    f = f ? f + 1 : tb;

    if (strncmp(f, VMLINUZ_PREFIX, sizeof(VMLINUZ_PREFIX) - 1) == 0) {
        return strdup(f + sizeof(VMLINUZ_PREFIX) - 1);
    }
    return NULL;
}