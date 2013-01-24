#/bin/sh

if [ -z $GBS_HOME ]; then
    echo "Error: GBS_HOME is not set, point it to the correct path of Griotte build system" >&2
    exit 1
fi

CMDLINE="$0 $*"
# Save current working directory
CWD=`pwd`
# Absolute path to GBS
GBS_HOME=`dirname $0`
cd $GBS_HOME
GBS_HOME=`pwd`

# Restore current working directory
cd $CWD


cat $GBS_HOME/template/gitignore  >/dev/null
if [ $? -ne 0 ]; then
    echo "Error: Unable to find Griotte build system (GBS) root directory" >&2
    exit 1
fi


usage() {
    echo "Usage:"
    echo "   ws [-create|-clear] [-git] PROJECTNAME"
}

username() {
    USERNAME=`whoami`

    GREP=`which grep`
    CUT=`which cut`
    
    if [ -n $GREP ] && [ -n $CUT ]; then
        USERNAME=`grep "^$USERNAME:" /etc/passwd | cut -d: -f5 | cut -d, -f1`
    fi
    
    if [ -z "$USERNAME" ]; then
	USERNAME=`whoami`
    fi
}

create() {
    if [ -e $PROJECT ]; then
	echo "Error: Directory already exists: $PROJECT" >&2
	exit 1
    fi

    DATE=`date`
    username

    mkdir $PROJECT
    cd $PROJECT
    mkdir .gbs
    mkdir .gbs/tests
    mkdir .gbs/$PROJECT
    cp $GBS_HOME/template/tests.pro .gbs/tests/tests.pro
    cp $GBS_HOME/template/project.pro .gbs/$PROJECT/$PROJECT.pro

    mkdir include
    mkdir include/$PROJECT
    mkdir src
    mkdir tests
    echo '#ifdef  __@PRJ@_H__'       > include/$PROJECT/$PROJECT.h
    echo '#define __@PRJ@_H__'      >> include/$PROJECT/$PROJECT.h
    echo ' '                        >> include/$PROJECT/$PROJECT.h
    echo '#endif /* __@PRJ@_H__ */' >> include/$PROJECT/$PROJECT.h
    cp $GBS_HOME/template/main.c src/main.c
    cp $GBS_HOME/template/test.c tests/test.c

    cd .gbs
    echo "#************************************************************"  > $PROJECT.pro
    echo "#* Generated automatically by '$0'                           " >> $PROJECT.pro
    echo "#* Command: $CMDLINE                                         " >> $PROJECT.pro
    echo "#* Author:  $USERNAME                                        " >> $PROJECT.pro
    echo "#* Date:    $DATE                                            " >> $PROJECT.pro
    echo "#************************************************************" >> $PROJECT.pro
    echo "TEMPLATE = subdirs                                           " >> $PROJECT.pro
    echo "CONFIG  += ordered                                           " >> $PROJECT.pro
    echo "SUBDIRS  = $PROJECT tests                                    " >> $PROJECT.pro
    cd ..

    # Prepare make.sh (for use from IDE, e.g. Eclipse)
    echo "#!/bin/sh"          > make.sh
    echo '$GBS_HOME/make.sh \$*' >> make.sh
    chmod +x make.sh

    # Prepare clean.sh
    echo "#!/bin/sh"                > clean.sh
    echo "cd .gbs"                 >> clean.sh
    echo '$GBS_HOME/make.sh clean' >> clean.sh
    chmod +x clean.sh
    
    # Prepare build.sh
    echo "#!/bin/sh"                > build.sh
    echo "cd .gbs"                 >> build.sh
    echo '$GBS_HOME/make.sh clean' >> build.sh
    echo '$GBS_HOME/make.sh all'   >> build.sh
    chmod +x build.sh

    # Prepare backup.sh
    echo "#!/bin/sh"                  > backup.sh
    echo "./clean.sh"                >> backup.sh
    echo "DATE=\`date +%Y%m%d\`"     >> backup.sh
    echo "tar -cjvf $PROJECT-\$DATE.tar.bz2 ./" >> backup.sh
    chmod +x backup.sh

    # Prepare .gitignore
    echo "!.gitignore"                > .gitignore
    cat $GBS_HOME/template/gitignore >> .gitignore
    echo ""                          >> .gitignore
    cat $GBS_HOME/template/clean.lst >> .gitignore

    touch README.md

    GIT_EXE=`which git`
    if [ "$GIT_OK" == "yes" ] && [ -x $GIT_EXE ] ; then
	$GIT_EXE init
	$GIT_EXE add -A .
	$GIT_EXE commit -m "Initial commit"
	$GIT_EXE remote add origin git@github.com:semenovf/${PROJECT}.git
	$GIT_EXE push -u origin master
    fi

    echo "Project '$PROJECT' created"
    echo "Modyfy '.gbs/$PROJECT.pro' to add new subprojects"
}

clean() {
    echo "Nothing to clean"
}


case "$1" in
    -create)
	PROJECT=$2
	GIT_OK=
	if [ "$PROJECT" == "-git" ] ; then
	    PROJECT=$3
	    GIT_OK=yes
	fi
	if [ -z $PROJECT ]; then
	    usage
	    exit 1
	fi
	create
	;;
    -clean)
	clean
	;;
    *)
	usage
	;;
esac

exit 0
