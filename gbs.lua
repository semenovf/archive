#!/usr/bin/lua

-- Application variables
local gbs_solutions_file = "solutions.txt";

function print_error (m)
    print("ERROR: " .. m);
end

function parse_option (opt)
    local optname, optarg = opt:match("^[-][-]([%a%d_-]-)=(.-)$");
    
    if optname == nil then
        optname = opt:match("^[-][-]([%a%d_-]-)$");
    end
    
    if optarg == nil or optarg:len() == 0 then
        optarg = nil;
    end

    return optname, optarg;
end

function is_valid_name (name)
    local r = name:match("^([%a%d_-]-)$");
    
    if r == nil then
        return false;
    end

    return true;
end

function file_exists (path)
    --if type(path)~="string" then return false end
    return os.rename(path,path) and true or false;
end

function touch_file (path)
    local fh, errstr = io.open(path, "w");
    if fh == nil then
        print_error(path .. ": can't touch file: " .. errstr); 
        return false;
    end
    io.close(fh);
    return true;
end

function create_dir (path)
    if not os.execute("mkdir " .. path) then
        return false;
    end
    return true;
end

--function change_dir (path)
--    if not os.execute("cd " .. path) then
--        print_error(path .. "failed to change directory");
--        return false;
--    end
--    return true;
--end

function usage ()
    print("NAME");
    print("    gbs - utitlity for prepare development environment");
    print("");
    print("SYNOPSYS");
    print("    gbs ACTION OPTIONS");
    print("");
    print("    (1) gbs help");
    print("    (2) gbs create-workspace WORKSPACE_PATH");
    print("    (3) gbs create-ws WORKSPACE_PATH");
    print("    (4) gbs create-solution SOLUTION_NAME [GIT_OPTIONS]");
    print("    (5) gbs create-sepaloid SEPALOID_NAME --sepaloid=SEPALOID_PREDEFINED_SET");
    print("    (6) gbs create-sepaloid SEPALOID_NAME [--petaloid=PETALOID_NAME [--petaloid=PETALOID_NAME ...]]");
    print("    (7) gbs create-project PROJECT_NAME --project-type=PROJECT_TYPE");
--    print("    (8) gbs --create-ws=PATH[-c|-create|--create] [-sepaloid|--sepaloid] [GIT_OPTIONS] SOLUTIONNAME");
    print("");
    print("DESCRIPTION");
    print("    (1)        - output this help and exit");
    print("    (2) or (3) - create workspace at specified directory WORKSPACE_PATH");
    print("    (4)        - create solution with name SOLUTION_NAME and type SOLUTION_TYPE");
    print("");
    print("ARGUMENTS");
    print("    WORKSPACE_PATH  - valid file system path for directory");
    print("    SOLUTION_NAME   - valid file system name for directory (not to be confused with full directory path)");
    print("    SEPALOID_NAME   - valid file system name for directory");
    print("    SEPALOID_PREDEFINED_SET - one of { default | gui-qt4 }");
    print("    PETALOID_NAME   - valid file system name for directory");
    print("    PROJECT_NAME    - valid file system name for directory");
    print("    PROJECT_TYPE    - one of { app | lib | static | example | test }");
    print("    GIT_OPTIONS     - options specific for 'git' revision control system. See 'GIT OPTIONS' section");
    print("");
    print("GIT OPTIONS");
    print("    --git");
    print("        initialize only git local repo");
    print("    --git-hosting-github");
    print("        initialize git local repo and synchronize it with remote repository"); 
    print("        on GitHub (must be created previously)");
    print("    --git-hosting-bitbucket");
    print("        initialize git local repo and synchronize it with remote repository");
    print("        on Bitbucket (must be created previously)");
end

function create_workspace (path)
    if file_exists(path) then
        print_error(path .. ": can't create workspace: directory already exists");
        return false
    end
    
    if not create_dir(path) then
        print_error(path .. ": failed to create workspace directory");
        return false; 
    end
    
--    if not change_dir(path) then
--        print_error(path .. ": failed to change the current directory to workspace directory");
--        return false; 
--    end
    
    local gbs_solutions_path = path .. "/" .. gbs_solutions_file;
    
    if not touch_file(gbs_solutions_path) then
        print_error(gbs_solutions_path .. ": can't create file");
        return false;
    end
    
    return true;
end

function create_solution (name)
    if not is_valid_name(name) then
        print_error("invalid solution name, only alphanumeric characters, underscore ('_') and dash ('-') are valid");
        return false;
    end
    
    if not file_exists(gbs_solutions_file) then
        print_error("can't create solution outside of workspace directory");
        return false;
    end
    
    if not create_dir(name) then
        print_error(name .. ": failed to create solution");
        return false; 
    end
    
    return true;
end

-- Check GBS_HOME environment variable is set
local gbs_home = os.getenv("GBS_HOME");

if gbs_home == nil or gbs_home:len() == 0 then
    print_error("GBS_HOME environment variable must be set to GBS directory");
    os.exit(1);
end

local gbs_action = arg[1];

if #arg == 0 then
    print_error("action must be specified, type `" .. arg[0] .. " help' to display usage.");
    os.exit(1);
elseif gbs_action == "help" then
    usage();
    os.exit(0);
elseif gbs_action == "create-workspace" or gbs_action == "create-ws" then
    local path = arg[2];
    if path == nil then
        print_error("workspace path must be specified");
        os.exit(1);
    end
    
    if not create_workspace(path) then
        os.exit(1);
    end
elseif gbs_action == "create-solution" then
    local name = arg[2];
    if name == nil then
        print_error("solution name must be specified");
        os.exit(1);
    end
    
    if not create_solution(name) then
        os.exit(1);
    end
else
    print_error("bad action, type `" .. arg[0] .. " help' to display usage.");
    os.exit(1);
end
 
os.exit(0);

--local args_index = 0;
--
--for i = 1, #arg do
--    if arg[i]:find("--", 1, true) == 1 then
--        if arg[i] == "--help" then
--            usage();
--            os.exit(0);
--        else
--            local opt, optarg = parse_option(arg[i]);
--            
--            if opt == "create-workspace" or opt == "create-ws" then
--                gbs_prefs.workspace_name = optarg;
--            else
--                print_error(arg[i] .. ": bad option");
--                os.exit(1);
--            end
--        end
--    else
--        args_index = args_index + 1;
--        gbs_prefs.args[args_index] = arg[i];
--    end
--end

--for i = 1, #gbs_prefs.args do
--    print(gbs_prefs.args[i]);
--end

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