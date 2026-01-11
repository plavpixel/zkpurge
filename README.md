# zkpurge

minimal C-lang utility to purge old ZFSBootMenu executables<br/>
based on Void's [vkpurge(8)](https://man.voidlinux.org/vkpurge)

## Compiling
build:
```bash
make clean build
```
install:
```bash
sudo make install
```
all-in-one:
```bash
sudo make all
```

## Usage

```bash
zkpurge list
sudo zkpurge rm 3.1.0_1
sudo zkpurge rm all
```

### environment

- `ZBM_PATH`: override auto-detected directory.
- `ZBM_PROTECT`: version string to keep during `rm all`.