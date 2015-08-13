require "pfs.sys.os";
require "pfs.die";
require "gbs.utils";

local fs = require("pfs.sys.fs");

local workspace = {};

function workspace:new ()
    local o = {}; 
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

function workspace:create (settings)
    local verbose           = settings:get("Verbose") or false;
    local path              = settings:get_or_throw("WorkspacePath");
    local buildTool         = settings:get_or_throw("build-tool");
    local targetPlatform    = settings:get_or_throw("target-platform");
    local workspaceFileName = settings:get_or_throw("WorkspaceFileName");
    local cmdlineString     = settings:get_or_throw("CommandLineString");
    local programName       = settings:get_or_throw("ProgramName");
    
    local workspaceFile = fs.join(path, ".gbs", workspaceFileName);
  
    local trn = require("gbs.transaction"):begin(verbose);
  
    trn:append(function (args) return _isValidBuildTool(args[1]) end, {buildTool}, "Valid build tool");
    trn:append(function (args) return _isValidTargetPlatform(args[1]) end, {targetPlatform}, "Valid target platform");
    trn:append(PathNotExists, {path}, "Workspace directory does not exist");
    trn:append(MakeDir, {path}, "Create workspace directory");
    trn:append(MakeDir, {fs.join(path, ".gbs")}, "Create workspace system subdirectory");
    trn:append(MakeDir, {fs.join(path, ".build")}, "Create workspace build subdirectory");

    trn:append(AppendLinesToFile, {
          workspaceFile
        , {
              utils.fileTitle(programName, cmdlineString)
            , "build-tool=" .. buildTool
            , "target-platform=" .. targetPlatform
        }}, "Update workspace configuration file: " .. workspaceFile);
    
    return trn:exec();
end

return workspace;
