function Gbs:doHelp ()
    self:usage();
    return true;
end

function Gbs:usage ()
    print("NAME");
    print("    gbs - utitlity for manage development environment");
    print("");
    print("SYNOPSYS");
    print("    gbs DOMAIN [ACTION [OPTIONS]]");
    print("");
    print("    (1) gbs help");
    print("    (2) gbs workspace create PATH");
    print("    (3) gbs ws create PATH");
    print("    (4) gbs solution create NAME [GIT_OPTIONS]");
    print("    (5) gbs sepaloid create NAME --sepaloid=SEPALOID_SET");
    print("    (6) gbs sepaloid create NAME [--petaloid=NAME [--petaloid=NAME ...]]");
    print("    (7) gbs project create NAME --project-type=PROJECT_TYPE");
--    print("    (8) gbs --create-ws=PATH[-c|-create|--create] [-sepaloid|--sepaloid] [GIT_OPTIONS] SOLUTIONNAME");
    print("");
    print("DESCRIPTION");
    print("    (1)        - output this help and exit");
    print("    (2) or (3) - create workspace at specified directory WORKSPACE_PATH");
    print("    (4)        - create solution with name SOLUTION_NAME and type SOLUTION_TYPE");
    print("");
    print("DOMAINS (and synonims)");
    print("    help");
    print("    workspace (ws)");
    print("    solution");
    print("    sepaloid");
    print("    project");
    print("");
    print("VALUES");
    print("    PATH         - valid file system path for directory");
    print("    NAME         - valid only alphanumeric characters, underscore ('_') and dash ('-')");
    print("    SEPALOID_SET - one of { default | gui-qt4 }");
    print("    PROJECT_TYPE - one of { console-app | gui-app | shared-lib | static-lib | example | test }");
    print("    GIT_OPTIONS  - options specific for 'git' revision control system. See 'GIT OPTIONS' section");
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
