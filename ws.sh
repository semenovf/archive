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

GIT_HOSTING_GITHUB=github
GIT_HOSTING_BITBUCKET=bitbucket

# Restore current working directory
cd $CWD


cat $GBS_HOME/template/gitignore  >/dev/null
if [ $? -ne 0 ]; then
    echo "Error: Unable to find Griotte build system (GBS) root directory" >&2
    exit 1
fi


usage() 
{
    echo "Usage:"
    echo "   ws [-c|-create|--create] [GIT_OPTIONS] PROJECTNAME"
    echo ""
    echo "GIT OPTIONS:"
    echo "    -git | --git        - initialize git local repo"
    echo "    -git-hosting-YYY"
    echo "    | --git-hosting-YYY - push to remote git hosting,"
    echo "                          where YYY = github | bitbucket"
}

username() 
{
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

create() 
{
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
    mkdir cppcheck
    cp $GBS_HOME/template/header.hpp include/$PROJECT/$PROJECT.hpp
    cp $GBS_HOME/template/main.cpp src/main.cpp
    cp $GBS_HOME/template/test.cpp tests/test.cpp
    cp $GBS_HOME/template/cppcheck/includes-file cppcheck/includes-file
    cp $GBS_HOME/template/cppcheck/sources-file cppcheck/sources-file
    cp $GBS_HOME/template/cppcheck/options cppcheck/options

    cd .gbs
    echo "#************************************************************"  > $PROJECT.pro
    echo "#* Generated automatically by '$0'"                            >> $PROJECT.pro
    echo "#* Command: $CMDLINE"                                          >> $PROJECT.pro
    echo "#* Author:  $USERNAME"                                         >> $PROJECT.pro
    echo "#* Date:    $DATE"                                             >> $PROJECT.pro
    echo "#************************************************************" >> $PROJECT.pro
    echo "TEMPLATE = subdirs"                                            >> $PROJECT.pro
    echo "CONFIG  += ordered"                                            >> $PROJECT.pro
    echo "SUBDIRS  = $PROJECT tests"                                     >> $PROJECT.pro
    cd ..

    # Prepare make.sh (for use from IDE, e.g. Eclipse)
    echo "#!/bin/sh"             > make.sh
    echo 'cd .gbs'              >> make.sh
    echo '$GBS_HOME/make.sh $*' >> make.sh
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
    echo "PREFIX=\`pwd\`"            >> backup.sh
    echo "tar -cjvf \${PREFIX}-\${DATE}.tar.bz2 ./" >> backup.sh
    chmod +x backup.sh

    # Prepare .gitignore
    echo "!.gitignore"                > .gitignore
    cat $GBS_HOME/template/gitignore >> .gitignore
    echo ""                          >> .gitignore
    cat $GBS_HOME/template/clean.lst >> .gitignore

    cp  $GBS_HOME/template/README.md README.md

    GIT_EXE=`which git`
    if [ "$GIT_OK" == "yes" ] && [ -x $GIT_EXE ] ; then
	$GIT_EXE init
	$GIT_EXE add -A .
	$GIT_EXE commit -m "Initial commit"

	if [ "$GIT_HOSTING" == "$GIT_HOSTING_GITHUB" ] ; then
	    $GIT_EXE remote add origin git@github.com:semenovf/${PROJECT}.git
	    $GIT_EXE push -u origin master
	elif [ "$GIT_HOSTING" == "$GIT_HOSTING_BITBUCKET" ] ; then
	    $GIT_EXE remote add origin https://semenovf@bitbucket.org/semenovf/${PROJECT}.git
	    $GIT_EXE push -u origin --all   # pushes up the repo and its refs for the first time
	    $GIT_EXE push -u origin --tags  # pushes up any tags
	fi
    fi

    echo "Project '$PROJECT' created"
    echo "Modyfy '.gbs/$PROJECT.pro' to add new subprojects"
}



while [ x$1 != x ] ; do
    case $1 in 
    -c | -create | --create)
	CREATE_OK=yes
        ;;
    -git | --git)
	GIT_OK=yes
        ;;
    -git-hosting-github | --git-hosting-github)
        GIT_OK=yes
        GIT_HOSTING=$GIT_HOSTING_GITHUB
        ;;
    -git-hosting-bitbucket | --git-hosting-bitbucket)
        GIT_OK=yes
        GIT_HOSTING=$GIT_HOSTING_BITBUCKET
        ;;
    *)
        PROJECT=$1
        ;;
    esac
    shift
done

if [ -z $PROJECT ]; then
    usage
    exit 1
fi

if [ "$CREATE_OK" == "yes" ] ; then
    create
    exit 0
fi

exit 1
