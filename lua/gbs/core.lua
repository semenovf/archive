require "pfs.die";
require "pfs.string";
require "pfs.cli.router";
require "pfs.sys.os";

local fs = require("pfs.sys.fs");
local Settings = require("pfs.settings"):new("gbs.");

Settings:set("GbsHomeDir"       , os.getenv("GBS_HOME"));
Settings:set("SolutionFileName" , "solution.gbs");
Settings:set("WorkspaceFileName", "workspace.txt");
Settings:set("ProjectFileName"  , "project.gbs");
Settings:set("SourcesDirName"   , "src");
Settings:set("TestsDirName"     , "tests");

-- Specifies the number of jobs (commands) to run simultaneously.
-- Gmake's option --jobs=JOBS (value `1' signals to ignore this option)
Settings:set("Jobs", 1); 

gbs = {};

function loadPreferences ()
    local workspaceFilePath = fs.join(".gbs", Settings:get("WorkspaceFileName"));
    
    for i = 1, 4 do
        if fs.exists(workspaceFilePath) then
            break;
        end
        workspaceFilePath = fs.join("..", workspaceFilePath);
    end
    
    if not fs.exists(workspaceFilePath) then
        return false;
    end

    for line in io.lines(workspaceFilePath) do
        if not line:match('^#') then
            local k, v = line:match('^([^=]-)=(.-)$');
            if k ~= nil then
                Settings:set(k, v);
            end
        end
    end
    return true;
end

function _getSolutionNameFromFile (solutionFile)
    local r = nil;
    for line in io.lines(solutionFile) do
        r = line:match('^solution%s"([^%s]-)"');
        if r ~= nil then
            break;
        end
    end
    return r;
end

function solutionName ()
    local solutionFileName  = Settings:get_or_throw("SolutionFileName"); 
    local solutionFile = fs.join(".gbs", solutionFileName);
    
    die(solutionFile .. ": solution file not found"):unless(fs.exists(solutionFile));
    local solutionName = _getSolutionNameFromFile(solutionFile);
    die(solutionFile .. ": unable to take solution name, solution file may be corrupted")
        :when(solutionName == nil);
    return solutionName;
end

--
-- Empty string equivalent to nil
--
function _esn (s)
    return string.isEmpty(s) and nil or s;
end


function gbs.run (argc, argv)
    local cli = require("pfs.cli.cli"):new();

    Settings:set("CommandLineString", cli.toString(#arg, arg));
    Settings:set("ProgramName", fs.basename(arg[0]));
    
    local status, msg = cli:parse(#arg, arg);
    die(msg):unless(status);
    
    loadPreferences();
    
    local help_type = require("gbs.help");

    cli:router()
        :b("verbose")
        :h(function (r)
                Settings:set("Verbose", true);
                return true;
           end)
        :continue();

    cli:router()
        :n("jobs", Settings:get("Jobs"))
        :h(function (r)
                Settings:set("Jobs", tonumber(r:optArg("jobs")));
                return true;
           end)
        :continue();
    
    cli:router()
        :b("dump")
        :h(function (r)
                print("Options: " .. cli:dumpOpts());
                print("Free arguments: " .. cli:dumpArgs());
           end);

    cli:router()
        :a("help")
        :a({"workspace", "ws", "solution", "sln", "project", "pro", "prj"})
        :h(function (r)
                Settings:set("Domain", r:actionAt(1));
                help_type:new(Settings):help();
                return true;
           end);
    
    cli:router()
        :a("help")
        :h(function (r) 
            help_type:new(Settings):help();
            return true;
        end);
--      TODO Really help must be generated automatically (need to implement)
--        :h(function (r) 
--                cli:guide():usage();
--                return true;
--           end);

    cli:router()
        :a({"workspace", "ws"})
        :b("create")
        :s("path")
        :s("build-tool")
        :s("target-platform", "")
        :s("config", "debug")
        :h(function (r)
                Settings:set("WorkspacePath" , r:optArg("path"));
                Settings:set("BuildTool"     , r:optArg("build-tool"));
                Settings:set("TargetPlatform", r:optArg("target-platform"));
                Settings:set("BuildConfig"   , r:optArg("config"));
                return require("gbs.workspace"):new(Settings):create();
           end);

    cli:router()
        :a({"workspace", "ws"})
        :b("build")
        :h(function (r)
                return require("gbs.workspace"):new(Settings):build();
           end);

    cli:router()
        :a({"workspace", "ws"})
        :b("clean")
        :h(function (r)
                return require("gbs.workspace"):new(Settings):clean();
           end);


    cli:router()
        :a({"solution", "sln"})
        :b("create")
        :s("name")
        :b("git", false)
        :h(function (r)
                Settings:set("SolutionName", r:optArg("name"));
                Settings:set("EnableGitRepo", r:optArg("git"));
                return require("gbs.solution"):new(Settings):create();
           end);

    cli:router()
        :a({"project", "pro", "prj"})
        :b("create")
        :s("name")
        :s("type", "console-app")
        :s("lang", "C++")
        :s("depends", {})
	:b("enable-qt", false)
        :h(function (r)
                Settings:set("SolutionName"       , solutionName());
                Settings:set("ProjectName"        , r:optArg("name"));
                Settings:set("ProjectType"        , r:optArg("type"));
                Settings:set("ProjectLanguage"    , r:optArg("lang"));
                Settings:set("ProjectDependencies", r:optArg("depends"));
		Settings:set("EnableQt"           , r:optArg("enable-qt"));
                return require("gbs.project"):new(Settings):create();
           end);

    cli:router()
        :a({"project", "pro", "prj"})
        :b("build")
        :s("name", "")
        :s("config", Settings:get("BuildConfig") or "")
        :s("build-tool", Settings:get("BuildTool") or "")
        :s("target-platform", Settings:get("TargetPlatform") or "")
        :h(function (r)
                Settings:set("ProjectName"   , _esn(r:optArg("name")));
                Settings:set("BuildConfig"   , _esn(r:optArg("config")));
                Settings:set("BuildTool"     , _esn(r:optArg("build-tool")));
                Settings:set("TargetPlatform", _esn(r:optArg("target-platform")));
                return require("gbs.project"):new(Settings):build();
           end);

    -- Synonym for `gbs project --build' 
    cli:router()
        :a("all")
        :h(function (r)
                Settings:set("ProjectName"   , nil);
                Settings:set("BuildConfig"   , Settings:get_or_throw("BuildConfig"));
                Settings:set("BuildTool"     , Settings:get_or_throw("BuildTool"));
                Settings:set("TargetPlatform", Settings:get_or_throw("TargetPlatform"));
                return require("gbs.project"):new(Settings):build();
           end);

    cli:router()
        :a({"project", "pro", "prj"})
        :b("clean")
        :s("name", "")
        :s("config", Settings:get("BuildConfig") or "")
        :s("build-tool", Settings:get("BuildTool") or "")
        :s("target-platform", Settings:get("TargetPlatform") or "")
        :h(function (r)
                Settings:set("ProjectName"   , _esn(r:optArg("name")));
                Settings:set("BuildConfig"   , _esn(r:optArg("config")));
                Settings:set("BuildTool"     , _esn(r:optArg("build-tool")));
                Settings:set("TargetPlatform", _esn(r:optArg("target-platform")));
                return require("gbs.project"):new(Settings):clean();
           end);

    -- Synonym for `gbs project --clean' 
    cli:router()
        :a("clean")
        :h(function (r)
                Settings:set("ProjectName"   , "");
                Settings:set("BuildConfig"   , Settings:get_or_throw("BuildConfig"));
                Settings:set("BuildTool"     , Settings:get_or_throw("BuildTool"));
                Settings:set("TargetPlatform", Settings:get_or_throw("TargetPlatform"));
                return require("gbs.project"):new(Settings):clean();
           end);
        
    cli:router()
        :h(function (r)
                print("Type `gbs help' for usage");
           end);

    if cli:run() then return 0; end
               
    return 1;
end
