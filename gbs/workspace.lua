local workspace = {};

require("gbs.sys.os");
require("gbs.sys.die");

local lib  = require("gbs.sys.lib");
local fs   = require("gbs.sys.fs");
local app  = require("gbs.sys.app");

function workspace:new ()
    local o = {}; 
    self.__index = self;
    return setmetatable(o, self);
end

function workspace.usage ()
    print("NAME");
    print("    gbs { workspace | ws } - workspace manipulation");
    print("");
    print("SYNOPSYS");
    print("    (1) gbs workspace --create");
    print("            --path=PATH");
    print("            --build-tool=BUILD_TOOL");
    print("            --target-platform=TARGET_PLATFORM");
    print("");
    print("DESCRIPTION");
    print("    (1) - create workspace at specified directory by PATH");
    print("");
    print("VALUES");
    print("    PATH");
    print("        Valid file system path for directory");
    print("    BUILD_TOOL");
    print("        `gmake' | `vs2005' | `vs2008' | `vs2010' | `vs2012' | `vs2013' | `vs2015'");
    print("    TARGET_PLATFORM"); 
    print("        `unix32' | `unix64' | `mswin32' | `mswin64'");
end

function _isValidBuildTool (bt)
    if bt == "gmake"
            or bt == "vs2005"
            or bt == "vs2008"
            or bt == "vs2010"
            or bt == "vs2012"
            or bt == "vs2013"
            or bt == "vs2015" then
        return true;
    end
    return false;
end

function _isValidTargetPlatform (tp)
    if tp == "unix32"
            or tp == "unix64"
            or tp == "mswin32"
            or tp == "mswin64" then
        return true;
    end
    return false;
end

function workspace:create (router)
--    local gbs  = self:gbs();
    
    local path = router:optArg("path");
    local buildTool = router:optArg("build-tool");
    local targetPlatform = router:optArg("target-platform");
    local workspaceFile = fs.join(path, ".gbs", gbs:workspaceFileName());
  
    die("invalid build tool"):unless(_isValidBuildTool(buildTool));
    die("invalid target platform"):unless(_isValidTargetPlatform(targetPlatform));
    die(path .. ": can't create workspace: path already exists"):when(fs.exists(path));

    if not (fs.mkdir(path)
            and fs.mkdir(fs.join(path, ".gbs"))
            and fs.mkdir(fs.join(path, ".build"))) then
        die(path .. ": can't create workspace"):now();
    end
    
    local cmdline = app:
    
    if fs.appendLines(workspaceFile, 
          "#************************************************************"
        , "#* Generated automatically by `" .. app:cmdline():name() .. "'"
        , "#* Command: `" .. app:cmdlineToString() .. "'"
        , "#* Date:    " .. os.date() 
        , "#************************************************************"
        , "build-tool=" .. buildTool
        , "target-platform=" .. targetPlatform
    ) or die("failed to update " .. workspaceFile):now() then end
    
    return true;
end

return workspace;
