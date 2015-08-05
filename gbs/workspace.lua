local workspace = {};

require("gbs.sys.os");

local lib  = require("gbs.sys.lib");
local fs   = require("gbs.sys.fs");
local app  = require("gbs.sys.app");

function workspace:new (gbs)
    local o = {
        gbs = function () return gbs; end
    }; 
    
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

function workspace:run ()
    local gbs = self:gbs();
    
    if gbs:hasOpt("create") then
        return self:create();
    else
        lib.print_error("action for workspace must be specified");
    end
    return false;
end

function workspace:create ()
    local gbs  = self:gbs();
    
    local path = gbs:optarg("path") or lib.die("`path' must be specified");
    local buildTool = gbs:optarg("build-tool") or lib.die("`build-tool' must be specified");
    local targetPlatform = gbs:optarg("target-platform") or lib.die("`target-platform' must be specified");
    local workspaceFile = fs.join(path, ".gbs", gbs:workspaceFileName());
  
    if not _isValidBuildTool(buildTool) then lib.die("invalid build tool"); end
    if not _isValidTargetPlatform(targetPlatform) then lib.die("invalid target platform"); end
    
    if fs.exists(path) then lib.die(path .. ": can't create workspace: path already exists"); end

    if not (fs.mkdir(path)
            and fs.mkdir(fs.join(path, ".gbs"))
            and fs.mkdir(fs.join(path, ".build"))) then
        lib.die(path .. ": can't create workspace");
    end
    
    if fs.appendLines(workspaceFile, 
          "#************************************************************"
        , "#* Generated automatically by `" .. app.name() .. "'"
        , "#* Command: `" .. app.cmdline() .. "'"
        , "#* Date:    " .. os.date() 
        , "#************************************************************"
        , "build-tool=" .. buildTool
        , "target-platform=" .. targetPlatform
    ) or lib.die("failed to update " .. workspaceFile ) then end
    
    return true;
end

return workspace;
