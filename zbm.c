#define _POSIX_C_SOURCE 200809L
#include "zbm.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

static void rm_f(const char* p) {
    struct stat st;
    if (stat(p, &st) == 0) {
        if (unlink(p) == 0) printf("Removed: %s\n", p);
        else fprintf(stderr, "Error: Failed to remove %s (%s)\n", p, strerror(errno));
    }
}

static void strip_efi_ext(char* ver) {
    char *dot = strrchr(ver, '.');
    if (dot && (strcmp(dot, UKI_SUFFIX_LOWER) == 0 || strcmp(dot, UKI_SUFFIX_UPPER) == 0)) {
        *dot = '\0';
    }
}

struct kernel_list zbm_scan_kernels(const char* p) {
    DIR* d = opendir(p);
    if (!d) { perror("opendir"); exit(EXIT_FAILURE); }

    size_t cap = 16, cnt = 0;
    struct kernel_entry* e = malloc(cap * sizeof(struct kernel_entry));
    if (!e) abort();

    struct dirent* de;
    while ((de = readdir(d))) {
        if (strncmp(de->d_name, VMLINUZ_PREFIX, sizeof(VMLINUZ_PREFIX) - 1)) continue;

        if (cnt >= cap) {
            cap *= 2;
            void* tmp = realloc(e, cap * sizeof(struct kernel_entry));
            if (!tmp) abort();
            e = tmp;
        }

        e[cnt].full_filename = strdup(de->d_name);
        e[cnt].version = strdup(de->d_name + sizeof(VMLINUZ_PREFIX) - 1);
        strip_efi_ext(e[cnt].version);
        cnt++;
    }
    closedir(d);

    qsort(e, cnt, sizeof(struct kernel_entry), entry_cmp);

    size_t unique_cnt = 0;
    if (cnt > 0) {
        size_t j = 0;
        for (size_t i = 1; i < cnt; i++) {
            if (strcmp(e[j].version, e[i].version) != 0) {
                j++;
                e[j] = e[i];
            } else {
                free(e[i].full_filename);
                free(e[i].version);
            }
        }
        unique_cnt = j + 1;
    }

    return (struct kernel_list){e, unique_cnt};
}

void zbm_free_list(struct kernel_list l) {
    for (size_t i = 0; i < l.count; i++) {
        free(l.entries[i].full_filename);
        free(l.entries[i].version);
    }
    free(l.entries);
}

void zbm_purge_version(const char* p, const char* v) {
    if (strchr(v, '/') || strstr(v, "..")) return;

    char path[MAX_PATH_LEN];

    if (snprintf(path, MAX_PATH_LEN, "%s/%s%s", p, VMLINUZ_PREFIX, v) < MAX_PATH_LEN) rm_f(path);
    if (snprintf(path, MAX_PATH_LEN, "%s/%s%s%s", p, INITRAMFS_PREFIX, v, INITRAMFS_SUFFIX) < MAX_PATH_LEN) rm_f(path);
    if (snprintf(path, MAX_PATH_LEN, "%s/%s%s%s", p, VMLINUZ_PREFIX, v, UKI_SUFFIX_LOWER) < MAX_PATH_LEN) rm_f(path);
    if (snprintf(path, MAX_PATH_LEN, "%s/%s%s%s", p, VMLINUZ_PREFIX, v, UKI_SUFFIX_UPPER) < MAX_PATH_LEN) rm_f(path);
}