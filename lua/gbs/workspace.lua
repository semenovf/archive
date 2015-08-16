require "pfs.sys.os";
require "pfs.string";
require "pfs.die";
require "gbs.utils";

local fs = require("pfs.sys.fs");

local workspace = {};

function workspace:new (settings)
    local o = { _settings = settings }; 
    self.__index = self;
    return setmetatable(o, self);
end

--function workspace.usage ()
--    print("NAME");
--    print("    gbs { workspace | ws } - workspace manipulation");
--    print("");
--    print("SYNOPSYS");
--    print("    (1) gbs workspace --create");
--    print("            --path=PATH");
--    print("            --build-tool=BUILD_TOOL");
--    print("            --target-platform=TARGET_PLATFORM");
--    print("");
--    print("DESCRIPTION");
--    print("    (1) - create workspace at specified directory by PATH");
--    print("");
--    print("VALUES");
--    print("    PATH");
--    print("        Valid file system path for directory");
--    print("    BUILD_TOOL");
--    print("        `gmake' | `vs2005' | `vs2008' | `vs2010' | `vs2012' | `vs2013' | `vs2015'");
--    print("    TARGET_PLATFORM"); 
--    print("        `unix32' | `unix64' | `mswin32' | `mswin64'");
--end

function workspace.isValidBuildTool (buildTool)
    if buildTool == "gmake"
            or buildTool == "vs2005"
            or buildTool == "vs2008"
            or buildTool == "vs2010"
            or buildTool == "vs2012"
            or buildTool == "vs2013"
            or buildTool == "vs2015" then
        return true;
    end
    return false;
end

function workspace.isValidTargetPlatform (platform)
    if platform == "unix32"
            or platform == "unix64"
            or platform == "mswin32"
            or platform == "mswin64" then
        return true;
    end
    return false;
end

function workspace:create ()
    local settings          = self._settings;
    
    local verbose           = settings:get("Verbose") or false;
    local path              = settings:get_or_throw("WorkspacePath");
    local buildTool         = settings:get_or_throw("BuildTool");
    local targetPlatform    = settings:get("TargetPlatform");
    local workspaceFileName = settings:get_or_throw("WorkspaceFileName");
    local cmdlineString     = settings:get_or_throw("CommandLineString");
    local programName       = settings:get_or_throw("ProgramName");
    
    if string.isEmpty(targetPlatform) then
        local osi = os.info();
        die("Unable to recognize OS type"):unless(osi.type);
        die("Unable to recognize OS bits"):unless(osi.bits);
        targetPlatform = tostring(osi.type) .. tostring(osi.bits);
        
        if verbose then print("Target platform detected: " ..  targetPlatform); end
    end
    
    
    local workspaceFile = fs.join(path, ".gbs", workspaceFileName);
  
    local trn = require("gbs.transaction"):begin(verbose);
  
    trn:Function(function () return workspace.isValidBuildTool(buildTool) end, "Validate build tool");
    trn:Function(function () return workspace.isValidTargetPlatform(targetPlatform) end, "Validate target platform");
    trn:PathNotExists(path, "Workspace directory does not exist");
    trn:MakeDir(path, "Create workspace directory");
    trn:MakeDir(fs.join(path, ".gbs"), "Create workspace system subdirectory");
    trn:MakeDir(fs.join(path, ".build"), "Create workspace build subdirectory");

    trn:AppendLinesToFile(workspaceFile
        , { utils.fileTitle("#", programName, cmdlineString)
            , "BuildTool=" .. buildTool
            , "TargetPlatform=" .. targetPlatform
        }, "Update workspace configuration file: " .. workspaceFile);
    
    return trn:exec();
end

return workspace;
