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
    print("    (3) gbs solution create NAME [GIT_OPTIONS]");
    print("    (4) gbs sepaloid create NAME --sepaloid=SEPALOID_SET");
    print("    (5) gbs sepaloid create NAME [--petaloid=NAME [--petaloid=NAME ...]]");
    print("    (6) gbs project create --name=NAME [--type=PROJECT_TYPE] [--lang=LANG]");
--    print("    (8) gbs --create-ws=PATH[-c|-create|--create] [-sepaloid|--sepaloid] [GIT_OPTIONS] SOLUTIONNAME");
    print("");
    print("DESCRIPTION");
    print("    (1) - output this help and exit");
    print("    (2) - create workspace at specified directory by PATH");
    print("    (3) - create solution with name NAME");
    print("");
    print("DOMAINS");
    print("    help");
    print("    workspace | ws");
    print("    solution | sln");
    print("    sepaloid");
    print("    project | pro");
    print("");
    print("VALUES");
    print("    PATH         - valid file system path for directory");
    print("    NAME         - valid only alphanumeric characters, underscore ('_') and dash ('-')");
    print("    SEPALOID_SET - `default' | `gui-qt4' }";
    print("    PROJECT_TYPE - `console-app' | `gui-app' | `shared-lib' | `static-lib' | `test'.
    print("                   Default is `console-app'");
    print("    LANG         - language identifier: C++ | C. Default is C++");
    print("    GIT_OPTIONS  - options specific for 'git' revision control system. See 'GIT OPTIONS' section");
    print("");
    print("GIT OPTIONS");
    print("    --git");
    print("        initialize only git local repo");
    print("    --git=github");
    print("        initialize git local repo and synchronize it with remote repository"); 
    print("        on GitHub (must be created previously)");
    print("    --git=bitbucket");
    print("        initialize git local repo and synchronize it with remote repository");
    print("        on Bitbucket (must be created previously)");
end
