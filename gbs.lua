#!/usr/bin/lua

gbs_solutions_file = "solutions.txt";

-- Check GBS_HOME environment variable is set
local gbs_home = os.getenv("GBS_HOME");

if gbs_home == nil or gbs_home:len() == 0 then
    print_error("GBS_HOME environment variable must be set to GBS directory");
    os.exit(1);
end

package.path = gbs_home .. '/?.lua;' .. package.path;

require "lua/utils"
require "lua/domain/help";
require "lua/domain/workspace";
require "lua/domain/solution";

local gbs_domain = arg[1];
local exit_code = 0;

if #arg == 0 then
    print_error("action must be specified, type `" .. arg[0] .. " help' to display usage.");
    exit_code = 1;
elseif gbs_domain == "help" then
    usage();
    exit_code = 0;
elseif gbs_domain == "workspace" or gbs_domain == "ws" then
    exit_code = workspace(2);
elseif gbs_domain == "solution" then
    exit_code = solution(2);
else
    print_error("bad domain, type `" .. arg[0] .. " help' to display usage.");
    exit_code = 1;
end
 
os.exit(exit_code);


--[[

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
#GIT_HOSTING_NPCPROM=npcprom

# Restore current working directory
cd $CWD


cat $GBS_HOME/template/gitignore  >/dev/null
if [ $? -ne 0 ]; then
    echo "Error: Unable to find Griotte build system (GBS) root directory" >&2
    exit 1
fi



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

prepare_simple_app()
{
    mkdir $SOLUTION
    cd $SOLUTION
    mkdir .gbs
    mkdir .gbs/tests
    mkdir .gbs/$SOLUTION
    cp $GBS_HOME/template/tests.pro .gbs/tests/tests.pro
    cp $GBS_HOME/template/project.pro .gbs/$SOLUTION/$SOLUTION.pro

    mkdir include
    mkdir include/$SOLUTION
    mkdir src
    mkdir tests
    mkdir cppcheck
    cp $GBS_HOME/template/header.hpp include/$SOLUTION/$SOLUTION.hpp
    cp $GBS_HOME/template/main.cpp src/main.cpp
    cp $GBS_HOME/template/test.cpp tests/test.cpp
    cp $GBS_HOME/template/cppcheck/includes-file cppcheck/includes-file
    cp $GBS_HOME/template/cppcheck/sources-file cppcheck/sources-file
    cp $GBS_HOME/template/cppcheck/options cppcheck/options

    cd .gbs
    echo "#************************************************************"  > solution.pro
    echo "#* Generated automatically by '$0'"                            >> solution.pro
    echo "#* Command: $CMDLINE"                                          >> solution.pro
    echo "#* Author:  $USERNAME"                                         >> solution.pro
    echo "#* Date:    $DATE"                                             >> solution.pro
    echo "#************************************************************" >> solution.pro
    echo "TEMPLATE = subdirs"                                            >> solution.pro
    echo "CONFIG  += ordered"                                            >> solution.pro
    echo "SUBDIRS  = $SOLUTION tests"                                    >> solution.pro
    cd ..
}

prepare_sepaloid()
{
    mkdir $SOLUTION
    cd $SOLUTION
    mkdir .gbs
    cp -R $GBS_HOME/template/sepaloid/.gbs/* .gbs/

    mkdir include
    mkdir src
    mkdir cppcheck
    cp -R $GBS_HOME/template/sepaloid/src/* src/
    cp -R $GBS_HOME/template/sepaloid/include/* include/
    cp $GBS_HOME/template/cppcheck/includes-file cppcheck/includes-file
    cp $GBS_HOME/template/cppcheck/sources-file cppcheck/sources-file
    cp $GBS_HOME/template/cppcheck/options cppcheck/options

    echo "#define APP_NAME \"$SOLUTION\"" > include/app_name.h

    cd .gbs
    mv common.pri common.pri~
    echo "GBS_TARGET_NAME=$SOLUTION" > common.pri
    cat common.pri~ >> common.pri
    rm common.pri~

    mv solution.pro solution.pro~
    echo "#************************************************************"  > solution.pro
    echo "#* Generated automatically by '$0'"                            >> solution.pro
    echo "#* Command: $CMDLINE"                                          >> solution.pro
    echo "#* Author:  $USERNAME"                                         >> solution.pro
    echo "#* Date:    $DATE"                                             >> solution.pro
    echo "#************************************************************" >> solution.pro
    cat solution.pro~ >> solution.pro
    rm solution.pro~

    cd ..
}

create()
{
    if [ -e $SOLUTION ]; then
	echo "Error: Directory already exists: $SOLUTION" >&2
	exit 1
    fi

    DATE=`date`
    username

    if [ "$SEPALOID_OK" == "yes" ] ; then
	prepare_sepaloid
    else
	prepare_simple_app
    fi

    # Install make.sh script (for use from IDE, e.g. Eclipse)
#    cp  $GBS_HOME/template/make.sh make.sh
#    chmod +x make.sh

    # Install clean script
    cp  $GBS_HOME/template/clean.sh clean.sh
    chmod +x clean.sh

    # Install build script
    cp  $GBS_HOME/template/build.sh build.sh
    chmod +x build.sh

    # Prepare backup script
    cp  $GBS_HOME/template/backup.sh backup.sh
    chmod +x backup.sh

    # Prepare cppcheck script
    cp  $GBS_HOME/template/cppcheck.sh cppcheck.sh
    chmod +x cppcheck.sh

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

	if   [ "$GIT_HOSTING" == "$GIT_HOSTING_GITHUB" ] ; then
	    $GIT_EXE remote add origin git@github.com:semenovf/${SOLUTION}.git
	    $GIT_EXE push -u origin master
	elif [ "$GIT_HOSTING" == "$GIT_HOSTING_BITBUCKET" ] ; then
	    #$GIT_EXE remote add origin https://semenovf@bitbucket.org/semenovf/${SOLUTION}.git
	    $GIT_EXE remote add origin git@bitbucket.org:semenovf/${SOLUTION}.git
#	    $GIT_EXE push -u origin --all   # pushes up the repo and its refs for the first time
#	    $GIT_EXE push -u origin --tags  # pushes up any tags
	    $GIT_EXE push -u origin master
#	elif [ "$GIT_HOSTING" == "$GIT_HOSTING_NPCPROM" ] ; then
#	    sh ../git.config.npcprom
#	    $GIT_EXE remote add origin git@gitlab:${SOLUTION}.git
#	    $GIT_EXE push -u origin master
	fi
    fi

    #
    # Add solution to list of solutions in the workspace
    #
    # cd to workspace directory
    cd ..
    if [ ! -f solutions.txt ] ; then
	touch solutions.txt
    fi
    echo "$SOLUTION" >> solutions.txt

    echo "Solution '$SOLUTION' created"
    echo "Modyfy '.gbs/solution.pro' to add new projects"
}



while [ x$1 != x ] ; do
    case $1 in 
    -c | -create | --create)
	CREATE_OK=yes
        ;;
    -sepaloid | --sepaloid)
	SEPALOID_OK=yes
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
#    -git-hosting-npcprom | --git-hosting-npcprom)
#        GIT_OK=yes
#        GIT_HOSTING=$GIT_HOSTING_NPCPROM
#        ;;
    *)
        SOLUTION=$1
        ;;
    esac
    shift
done

if [ -z $SOLUTION ]; then
    usage
    exit 1
fi

if [ "$CREATE_OK" == "yes" ] ; then
    create
    exit 0
fi

exit 1
--]]