# gid

> A terminal utility for managing multiple Git configurations.

## Requirements

`gid` requires no libraries or other external dependencies other than the
CMake build system and a C99 compiler.

## Building

Create a `build` directory inside the project's root folder, and navigate
to it.

```console
mkdir build
cd build
```

From the `build` directory, run CMake.

```console
cmake -DCMAKE_BUILD_TYPE=Release ..
```

Then, compile with Make (or Ninja, if including `-G Ninja` in the CMake
command).

```console
make
```

Or, install to CMake's default install directory of `/usr/local/bin`.

```console
make install
```

## Usage

### Configuration

To use gid, a configuration file named `configuration.gid` must be made
either in the same directory as the binary executable, or in the local
`$HOME/.config/gid` directory.

This configuration file has a very simple syntax, shown below.

```
start : my_config_name
user.name : my_git_name
user.email : my_git_email
user.signingkey : my_gpg_key
ssh_key_path : ~/.ssh/id_rsa

default : my_config_name_2
user.name : my_git_name2
user.email : my_git_email2
user.signingkey : my_gpg_key2
ssh_key_path : ~/.ssh/id_rsa2
```

Each Git configuration must be started by either `start` or `default`, in its
own line. `default` denotes the Git configuration to be used by default for
`gid`. Both `start` and `default` must be followed by a name for the
configuration, after a `:` delimiter.

Each Git configuration can support up to five different settings:

- `user.name`
- `user.email`
- `user.signingkey`
- `commit.template`
- `ssh_key_path`

Each setting is given a value after a `:` delimiter. All settings are named
equivalently to the Git configuration setting, except for `ssh_key_path`; the
`ssh_key_path` allows usage of a specific private SSH key for each Git
configuration.

### Running

After a proper configuration file has been made, Git commands in can be run
with the `gid` executable.

```console
gid add .
gid commit -S
```

Using `gid` instead of `git` will preserve all local/global Git settings with
the exception of the settings specified in the `gid` configuration file. These
`gid` configuration settings will overwrite Git configuration on a per-call
basis, instead of permanently modifying the local/global Git configuration.

To use a different Git configuration, simply add the configuration name as the
**first** argument after the executable.

```console
gid my_config_name commit -S
```

