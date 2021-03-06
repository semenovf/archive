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

function workspace.isValidBuildTool (buildTool)
    if buildTool == "gmake"
            or buildTool == "gmake2"
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
            or platform == "mswin64"
            or platform == "elbrus4c" then
        return true;
    end
    return false;
end

function workspace.isValidConfig (config)
    if config == "debug"
            or config == "release" then
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
    local config            = settings:get_or_throw("BuildConfig");

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
    trn:Function(function () return workspace.isValidConfig(config) end, "Validate config (debug or release)");
    trn:PathNotExists(path, "Workspace directory does not exist");
    trn:MakeDir(path, "Create workspace directory");
    trn:MakeDir(fs.join(path, ".gbs"), "Create workspace system subdirectory");
    trn:MakeDir(fs.join(path, ".build"), "Create workspace build subdirectory");

    trn:AppendLinesToFile(workspaceFile
        , { utils.fileTitle("#", programName, cmdlineString)
            , "BuildTool="      .. buildTool
            , "TargetPlatform=" .. targetPlatform
            , "BuildConfig="    .. config
        }, "Update workspace configuration file: " .. workspaceFile);
    
    return trn:exec();
end

--
-- actionStr := { "build" | "clean" }
--
function workspace:_make (actionStr, actionDescription)
    local settings          = self._settings;
    
    local verbose        = settings:get("Verbose") or false;
    local buildTool      = settings:get_or_throw("BuildTool");
    local targetPlatform = settings:get("TargetPlatform");
    local config         = settings:get_or_throw("BuildConfig");
    
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
    trn:Function(function () return workspace.isValidConfig(config) end, "Validate config (debug or release)");
    trn:PathExists(fs.join(".gbs", settings:get_or_throw("WorkspaceFileName")), "Inside workspace directory");
    trn:PathExists("solutions.txt", "Found list of solutions to build");
    trn:Function(function ()
        local file = assert(io.open("solutions.txt", "r"));

        for solution in file:lines() do
            if solution:len() > 0 then -- skip empty lines
                solution = string.trim(solution);
        
                if solution:len() > 0 then
                    if not solution:match("^#") then -- skip comment lines
                        local command = "cd " .. solution .. " && " .. "gbs pro --" .. actionStr;
                        if verbose then  command = command .. " --verbose"; end
                        if not fs.execute(command) then return false; end
                    end
                end
            end
        end
        return true;
    end, actionDescription);
    
    return trn:exec();
end

function workspace:build ()
    return self:_make("build", "Build solutions");
end

function workspace:clean ()
    return self:_make("clean", "Clean solutions");
end

return workspace;
