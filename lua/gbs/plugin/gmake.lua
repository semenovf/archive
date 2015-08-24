require "pfs.string";
require "gbs.utils";

local fs  = require("pfs.sys.fs");

local plugin = {};

function plugin:new (stage, settings)
    local o = {
          _stage = stage
        , _settings = settings
    }; 
    self.__index = self;
    return setmetatable(o, self);
end

function plugin:make (stage)
    local settings         = self._settings;
    local verbose          = settings:get("Verbose") or false;
    local projectName      = settings:get("ProjectName");
    local buildConfig      = settings:get_or_throw("BuildConfig");
    local buildTool        = settings:get_or_throw("BuildTool");
    local targetPlatform   = settings:get_or_throw("TargetPlatform");
    local solutionFileName = settings:get_or_throw("SolutionFileName");

    local solutionFile = fs.join(".gbs", solutionFileName);
    local configOpt    = "config=" .. buildConfig .. "_" .. targetPlatform;
    local verboseOpt   = verbose and "verbose=yes" or "";
    
    local gmakeTarget = "all";
    
    if stage == "build" then
        gmakeTarget  = string.isEmpty(projectName) and "all" or projectName;
    elseif stage == "clean" then
        gmakeTarget  = "clean";
    end 
    
    local trn = require("gbs.transaction"):begin(verbose);

    trn:Function(function () 
        return fs.executePremake("--file=" .. solutionFile, buildTool);
    end);

    if string.isEmpty(projectName) then
        trn:Function(function () 
            return fs.execute("make"
                , "-C", ".gbs"
                , configOpt
                , verboseOpt
                , gmakeTarget);
        end);
    else
        print(configOpt
                , verboseOpt
                , gmakeTarget);
        trn:Function(function ()
            fs.execute("make"
                , "-C", ".gbs"
                , configOpt
                , verboseOpt
                , gmakeTarget);
        end);
    end

    return trn;
end

function plugin:transaction ()
    if self._stage == "build" then
        return self:make("build");
    elseif self._stage == "clean" then
        return self:make("clean");
    end
    
    return nil;
end

return plugin;
