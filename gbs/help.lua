local help = {};

local lib   = require("gbs.sys.lib");

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
    print("    gbs DOMAIN [OPTIONS] [--dump]");
    print("");
    print("    (1) gbs help");
--    print("    () gbs sepaloid create --name=NAME --sepaloid=SEPALOID_SET");
--    print("    () gbs sepaloid create --name=NAME");
--    print("            [--petaloid=NAME [--petaloid=NAME ...]]");
--    print("    () gbs petaloid create --name=NAME");
    print("");
    print("DESCRIPTION");
    print("    (1) - output this help and exit");
    print("");
    print("VALUES");
    print("    SEPALOID_SET");
    print("        `default' | `gui-qt4'");
    print("");
    print("OPTIONS");
    print("    --dump");
    print("        dump options and exit execution");
end

function help:help (domain)
    if domain == "workspace" 
            or domain == "ws" then
        require("gbs.workspace").usage();
    elseif domain == "solution" 
            or domain == "sln" then
        require("gbs.solution").usage();
    elseif domain == "project" 
            or domain == "prj" 
            or domain == "pro" then
        require("gbs.project").usage();
    else
        lib.print_error("bad domain for help");
        return false;
    end 
    
    return true;
end

return help;

