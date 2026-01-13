# credit to github/WladimirBec for makefile structure
DESTDIR ?=
PREFIX  ?= /usr
BLDD    := $(abspath ./bld)

CC     ?= clang
CFLAGS := -Wall -Wextra -Werror \
		  -D_POSIX_C_SOURCE=200809L \
		  -std=c23 \
		  $(if $(filter 1,$(DEBUG)),-O0 -g,-O3 -flto -pipe)
LDFLAGS := $(if $(filter 1,$(DEBUG)),,-static)
SRCS   := main.c util.c zbm.c
OBJS   := $(patsubst %.c,$(BLDD)/%.o,$(SRCS))
TARGET := zkpurge

$(BLDD)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(BLDD):
	@ mkdir -p $@

$(OBJS): $(BLDD)/%.o: %.c | $(BLDD)
	$(CC) $(CFLAGS) -c $< -o $@

install: $(BLDD)/$(TARGET)
	install -Dm755 $< $(DESTDIR)$(PREFIX)/bin/$(TARGET)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(TARGET)

clean:
	rm -fr $(BLDD)
	rm -f compile_commands.json

compdb:
	@ ( \
		echo "["; \
		for src in $(SRCS); do \
			echo "  {"; \
			echo "    \"directory\": \"$(BLDD)\","; \
			echo "    \"file\": \"../$$src\","; \
			echo "    \"command\": \"$(CC) $(CFLAGS) -c $$src -o $(BLDD)/$${src%.c}.o\""; \
			echo "  },"; \
		done; \
		echo "]"; \
	) > ./compile_commands.json

.PHONY: install uninstall clean compdb