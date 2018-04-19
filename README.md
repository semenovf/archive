### NAME

`gbs` - build system based on premake5

### VERSION
Version 2.00b

### INSTALLATION
#### Linux (Ubuntu)

Run on terminal:

    $ sudo apt-get install lua5.2  

    $ cd /path/to/root-dir-for-gbs
    $ git clone git@github.com:semenovf/gbs.git
    $ # -- Set environment variable `GBS_HOME` to 'gbs' home directory.  
    $ export GBS_HOME=/path/to/root-dir-for-gbs/gbs
    $ mkdir ~/bin
    $ cd ~/bin
    $ ln -s $GBS_HOME/gbs.lua gbs

#### Prepare and Build premake-core
###### Download as submodule
    $ git submodule add git@github.com:semenovf/premake-core.git # once
    $ git submodule init
    $ git submodule update

###### Build

`premake` building consists of two stages:  

1. building `premake_bootstrap`
2. building `premake` using `premake_bootstrap` with predefined premake projects
   including third-party libraries: curl, libzip, lua, zlib.

In some cases `curl` may be excluded from building since it depends on 
`openssl` library development package (it is not installed by default on Ubuntu).
For this need to apply some changes in `Bootstrap.mak`:

* find appropriate to OS rule like 'OSNAME: $(SRC)' (OSNAME => `linux` for linux-family OSes,
  OSNAME => `osx` for MacOS etc).
* add `--no-curl` to line  
    `./build/bootstrap/premake_bootstrap --to=build/bootstrap gmake`  
so line will look like this  
    `./build/bootstrap/premake_bootstrap --to=build/bootstrap gmake --no-curl`

Now it is time to build and install `premake` directly (for `linux` as example):

    $ make -f Bootstrap.mak linux
    $ sudo cp bin/release/premake5 /usr/local/bin

For specific compiler replace first instruction with:

    $ CC=YOUR_COMPILER_NAME make -f Bootstrap.mak linux

For Elbrus compiler this instruction will look like:

    $ CC=lcc make -f Bootstrap.mak linux

#### Qt support
    $ git submodule add git@github.com:dcourtois/premake-qt.git # once
    $ git submodule init
    $ git submodule update

#### Finalize

Log out and log in again. Run terminal and check `gbs` installed properly:

    $ gbs
    Type `gbs help' for usage

### AUTHOR

Fedor Semenov, < fedor.v.semenov at gmail.com >

### SUPPORT

This software is under development.  
No documentation prepared yet.  

### LICENSE AND COPYRIGHT

Copyright 2012-2017 Fedor Semenov.
