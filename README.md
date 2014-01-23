###NAME

`gbs` - Griotte Build System based on qmake version 2.x.

###VERSION
Version 1.00b

###INSTALLATION
#### Linux (Ubuntu)

    $ sudo apt-get install g++  
    $ sudo apt-get install qt4-qmake  
    $ sudo apt-get install qt4-default  
    $ sudo touch /etc/ld.so.conf.d/devel.conf
    $ # -- Edit devel.conf
    $ cat /etc/ld.so.conf.d/devel.conf
    /home/user/build/unix/x86/debug
    /home/user/build/unix/x86/release
      
    $ cd /path/to/root-dir-for-gbs
    $ git clone git@github.com:semenovf/gbs.git
    $ # -- Set environment variable `GBS_HOME` to 'gbs' home directory.  
    $ echo "export GBS_HOME=/path/to/root-dir-for-gbs/gbs" >> ~/.profile
    $ # -- Set environment variable `GBS_PDIR` to root directory for projects.  
    $ echo "export GBS_PDIR=/path/to/root-dir-for-projects" >> ~/.profile
      
    $ # -- Test installation
    $ cd $GBS_PDIR
    $ $GBS_HOME/ws.sh -create myproject  
      
    Project 'myproject' created  
    Modify '.gbs/myproject.pro' to add new subprojects  
      
    $ cd $GBS_PDIR/myproject
    $ ./build.sh
      
    Reading /path/to/myproject/.gbs/myproject/myproject.pro  
    Reading /path/to/myproject/.gbs/tests/tests.pro  
    cd myproject/ && make -f Makefile distclean  
    ...  
    make[1]: Entering directory `/path/to/myproject/.gbs/myproject'  
    g++ -c -pipe -O0 -Werror ...  
    ...
    
On successful installation `build.sh` script will attempt to run `g++` compiler.


###AUTHOR

Fedor Semenov, < fedor.v.semenov at gmail.com >

###SUPPORT

This software is under development.  
No documentation prepared yet.  

###LICENSE AND COPYRIGHT

Copyright 2012-2014 Fedor Semenov.
