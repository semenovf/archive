#!/usr/bin/lua

-- Check GBS_HOME environment variable is set
local gbs_home = os.getenv("GBS_HOME");

if gbs_home == nil or gbs_home:len() == 0 then
    print_error("GBS_HOME environment variable must be set to GBS directory");
    os.exit(1);
end

package.path = gbs_home .. '/?.lua;' .. package.path;

local gbs_class = require("gbs.core");

--local router = require("gbs.sys.router"):new();
--router:a("ws"):b("create"):s("path"):dump();

Gbs = gbs_class:new();
Gbs:loadPrefs();
if Gbs:parseCommandLine(#arg, arg) then
    return Gbs:runDeprecated();
end
