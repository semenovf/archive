require("gbs.sys.die");
local lib   = require("gbs.sys.lib");

local help = {};

function help:new (o)
    local o = o or {};
    self.__index = self;
    return setmetatable(o, self);
end

function help:usage ()
    print("NAME");
    print("    gbs - utitlity for manage development environment.");
    print("");
    print("SYNOPSYS");
    print("    gbs DOMAIN [OPTIONS] [--dump]");
    print("");
    print("DOMAINS");
    print("    help      - output this help or usage for specified domain");
    print("    workspace - workspace manipulation");
    print("    solution  - solution manipulation");
    print("    project   - project manipulation");
--    print("    () gbs sepaloid create --name=NAME --sepaloid=SEPALOID_SET");
--    print("    () gbs sepaloid create --name=NAME");
--    print("            [--petaloid=NAME [--petaloid=NAME ...]]");
--    print("    () gbs petaloid create --name=NAME");
    print("");
--    print("VALUES");
--    print("    SEPALOID_SET");
--    print("        `default' | `gui-qt4'");
--    print("");
    print("OPTIONS");
    print("    --dump");
    print("        dump options and exit execution");
    print("NOTE");
    print("    For usage of specific domain execute `gbs help DOMAIN'.");
    print("");
    print("COPYRIGHT");
    print("    Copyright (C) 2015 Fedor Semenov.");
end

function help:help ()
    die("Expected domain hor help"):unless(self.domain);
    
    if self.domain == "workspace" 
            or self.domain == "ws" then
        require("gbs.workspace").usage();
    elseif self.domain == "solution" 
            or self.domain == "sln" then
        require("gbs.solution").usage();
    elseif help.domain == "project" 
            or self.domain == "prj" 
            or self.domain == "pro" then
        require("gbs.project").usage();
    else
        warn("bad domain for help");
        return false;
    end 
    
    return true;
end

return help;

