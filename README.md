# gid

![gid logo](assets/logo.png)

> A terminal utility for managing multiple Git profiles.

`gid` is a terminal Git profile manager that acts as an alias for the `git`
command. Rather than changing local/global Git configuration files, `gid`
directly inserts custom Git configurations into each call. `gid` is ideal
for users that desire a minimally invasive but convenient way to manage
multiple Git identities.

## Requirements

`gid` requires no libraries or external dependencies if using the pre-built
binaries. If building, it simply requires a working C99 compiler (or MSVC) and
the CMake build system.

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

To use gid, a configuration file must be made. If an environment variable named
`GID_CONFIGURATION` exists, then the file pointed to by that environment
variable will be used as the configuration file. Otherwise, the same directory
as the binary executable will be searched for a file named `configuration.gid`.
If not found, then finally the local `$HOME/.config/gid` directory will be
searched for a file named `configuration.gid`.

This configuration file has a very simple syntax, shown below.

```
start : my_profile_name
user.name : my_git_name
user.email : my_git_email
user.signingkey : my_gpg_key
ssh_key_path : ~/.ssh/id_rsa

default : my_profile_name_2
user.name : my_git_name2
user.email : my_git_email2
user.signingkey : my_gpg_key2
ssh_key_path : ~/.ssh/id_rsa2

start: my_profile_name_3
user.name :my_git_name3
commit.gpgsign: true
commit.template: ~/commit_template_one.txt
```

Each Git profile must be started by either `start` or `default`, in its
own line. `default` denotes the Git profile to be used by default for
`gid`, and only one `default` should exist in the configuration file. Both
`start` and `default` must be followed by a name for the profile, after a `:`
delimiter.

Each Git profile can support up to seven different settings:

- `user.name`
- `user.email`
- `user.signingkey`
- `commit.gpgsign`
- `commit.template`
- `pull.rebase`
- `ssh_key_path`

Each setting is given a value after a `:` delimiter. All settings are named
equivalently to the Git configuration setting, except for `ssh_key_path`; the
`ssh_key_path` allows usage of a specific private SSH key for each Git
profile.

### Running

After a proper configuration file has been made, Git commands can be run with
the `gid` executable.

```console
gid add .
gid commit -S
```

Using `gid` instead of `git` will inherit all local/global Git settings with
the exception of the settings specified in the `gid` configuration file. These
`gid` configuration settings will overwrite Git configuration on a per-call
basis, instead of permanently modifying the local/global Git configuration.

To use a different Git profile, simply add the profile name as the **first**
argument after the executable.

```console
gid my_profile_name commit -S
```

To change the Git configuration file according to a profile instead of calling
a Git command directly, `gidc` can be used:

```console
gidc set local my_profile_name
gidc set global my_profile_name_2
```

As of now, `gidc` can only be called with an accompanying `set`. Depending on
whether `set` is followed by `local` or `global`, `gidc` will overwrite the
respective local or global Git configuration according to the selected profile
values.
