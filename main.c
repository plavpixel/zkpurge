#define _POSIX_C_SOURCE 200809L
#include "defs.h"
#include "zbm.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    const char *env = getenv("ZBM_PATH");
    const char *dir = env ? env : detect_zbm_path();

    if (argc < 2) {
        fprintf(stderr, "Usage: %s list | rm all | rm <ver...>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "list") == 0) {
        struct kernel_list l = zbm_scan_kernels(dir);
        for (size_t i = 0; i < l.count; i++) printf("%s\n", l.entries[i].version);
        zbm_free_list(l);
    }
    else if (strcmp(argv[1], "rm") == 0) {
        if (!is_root()) {
            fprintf(stderr, "Error: Root required.\n");
            return EXIT_FAILURE;
        }
        if (argc < 3) return EXIT_FAILURE;

        if (strcmp(argv[2], "all") == 0) {
            struct kernel_list l = zbm_scan_kernels(dir);
            if (!l.count) return EXIT_SUCCESS;

            char *p_env = getenv("ZBM_PROTECT");
            char *p_sym = get_symlink_target_version(dir, "vmlinuz");

            printf("Keeping latest: %s\n", l.entries[l.count - 1].version);
            if (p_env) printf("Protecting pinned: %s\n", p_env);
            if (p_sym) printf("Protecting symlink: %s\n", p_sym);

            for (size_t i = 0; i < l.count - 1; i++) {
                char* v = l.entries[i].version;
                if ((p_env && strcmp(v, p_env) == 0) || (p_sym && strcmp(v, p_sym) == 0)) {
                    continue;
                }
                zbm_purge_version(dir, v);
            }

            if (p_sym) free(p_sym);
            zbm_free_list(l);
        } else {
            for (int i = 2; i < argc; i++) zbm_purge_version(dir, argv[i]);
        }
    }
    return EXIT_SUCCESS;
}