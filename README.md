# zkpurge

minimal C-lang utility to purge old ZFSBootMenu executables<br/>
based on Void's [vkpurge(8)](https://man.voidlinux.org/vkpurge)

## Compiling
Clone the repo:
```shell
git clone https://github.com/plavpixel/zkpurge.git && cd zkpurge
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
zkpurge list
sudo zkpurge rm 3.1.0_1
sudo zkpurge rm all # won't remove latest, can be manually overridden
```

#### Environment:

- `ZBM_PATH`: override auto-detected directory.
- `ZBM_PROTECT`: version string to keep during `rm all`.