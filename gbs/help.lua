local help = {};

function help:new ()
    local o = {};
    self.__index = self;
    return setmetatable(o, self);
end

function help:usage ()
    print("NAME");
    print("    gbs - utitlity for manage development environment");
    print("");
    print("SYNOPSYS");
    print("    gbs DOMAIN [ACTION [OPTIONS]] [--dump]");
    print("");
    print("    (1) gbs help");
    print("    (2) gbs workspace create --path=PATH");
    print("            --build-tool=BUILD_TOOL");
    print("            [--target-platform=TARGET_PLATFORM]");
    print("    (3) gbs solution create --name=NAME [GIT_OPTIONS]");
    print("    (4) gbs project create --name=NAME [--type=PROJECT_TYPE]");
    print("            [--lang=LANG] [--depends=NAME [--depends=NAME ...]]");
    print("    (5) gbs project build --name=NAME");
    print("            [--config={debug | release}]");
    print("            [--build-tool=BUILD_TOOL]");
    print("            [--target-platform=TARGET_PLATFORM]");
--    print("    () gbs sepaloid create --name=NAME --sepaloid=SEPALOID_SET");
--    print("    () gbs sepaloid create --name=NAME");
--    print("            [--petaloid=NAME [--petaloid=NAME ...]]");
--    print("    () gbs petaloid create --name=NAME");
    print("");
    print("DESCRIPTION");
    print("    (1) - output this help and exit");
    print("    (2) - create workspace at specified directory by PATH");
    print("    (3) - create solution with name NAME");
    print("    (4) - create project with name NAME");
    print("    (5) - build project with NAME");
    print("");
    print("DOMAINS");
    print("    help");
    print("    workspace | ws");
    print("    solution | sln");
    print("    sepaloid");
    print("    project | pro");
    print("");
    print("VALUES");
    print("    PATH");
    print("        Valid file system path for directory");
    print("    NAME");
    print("        Valid only alphanumeric characters, underscore ('_') and dash ('-')");
    print("    SEPALOID_SET");
    print("        `default' | `gui-qt4'");
    print("    PROJECT_TYPE");
    print("        `console-app' | `gui-app' | `shared-lib' | `static-lib' | `test'");
    print("        Default is `console-app'");
    print("    LANG");
    print("        Language identifier: `C++' | `C'. Default is C++");
    print("    GIT_OPTIONS");
    print("        Options specific for 'git' revision control system.");
    print("        See 'GIT OPTIONS' section");
    print("    BUILD_TOOL");
    print("        `gmake' | `vs2005' | `vs2008' | `vs2010' | `vs2012' | `vs2013' | `vs2015'");
    print("        default is 'gmake' for unix systems, for other systems must be specified");
    print("    TARGET_PLATFORM"); 
    print("        unix32 | unix64 | mswin32 | mswin64");
    print("");
    print("OPTIONS");
    print("    --dump");
    print("        dump options and exit execution");
    print("    --depend");
    print("        specify one more sibling project names (inside solution)");
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
    print("    --git-login=LOGIN");
    print("        login name to access remote git repository");
end

return help;

