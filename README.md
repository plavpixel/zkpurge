
# zkpurge

minimal C-lang utility to purge old ZFSBootMenu executables<br/>
based on Void's [vkpurge(8)](https://man.voidlinux.org/vkpurge)

## Compiling
Clone the repo:
```shell
git clone [https://github.com/plavpixel/zkpurge.git](https://github.com/plavpixel/zkpurge.git) && cd zkpurge
```

Build:
```sh
make
```

Install:
```sh
sudo make install
```

Uninstall:
```sh
sudo make uninstall
```

## Usage
```bash
# list available kernels
zkpurge -l

# remove a specific version
sudo zkpurge -r 3.1.0_1

# remove all except latest and pinned
sudo zkpurge -r all
```

### Options
- `-l`, `--list`: List available ZBM kernels.
- `-r`, `--remove`: Remove kernels (requires target).
- `-v`, `--version`: Show version and attribution.
- `-h`, `--help`: Show usage help.

### Environment Overrides
- `ZBM_PATH`: Override auto-detected directory.
- `ZBM_PROTECT`: Version string to keep during `-r all`.

## License
Public Domain