require "pfs.die";
require "pfs.string";
require "pfs.cli.router";

local fs = require("pfs.sys.fs");
local Settings = require("pfs.settings"):new("gbs.");

Settings:set("GbsHomeDir"       , os.getenv("GBS_HOME"));
Settings:set("SolutionFileName" , "solution.gbs");
Settings:set("WorkspaceFileName", "workspace.txt");
Settings:set("ProjectFileName"  , "project.gbs");
Settings:set("SourcesDirName"   , "src");
Settings:set("TestsDirName"     , "tests");

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

function _selectStringValue (initial, key)
    die("String expected"):unless(type(initial) == "string");
    if string.isEmpty(initial) then
        initial = Settings:get(key) 
            or die("`" .. key .. "' must be specified for workspace\n"):now();
    end
    return initial;
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
        :b("dump")
        :h(function (r)
                print("Options: " .. cli:dumpOpts());
                print("Free arguments: " .. cli:dumpArgs());
           end);

    cli:router()
        :a("help")
        :a({"workspace", "ws", "solution", "sln", "project", "pro", "prj"})
        --:a({})
        :h(function (r)
                Settings:set("Domain", r:actionAt(1));
                help_type:new(Settings):help();
                return true;
           end);
    
    cli:router()
        :a("help")
        :h(function (r) 
                cli:guide():usage();
                return true;
           end);
           
    cli:router()
        :a({"workspace", "ws"})
        :b("create")
        :s("path")
        :s("build-tool")
        :s("target-platform", "")
        :h(function (r)
                Settings:set("WorkspacePath", r:optArg("path"));
                Settings:set("BuildTool", r:optArg("build-tool"));
                Settings:set("TargetPlatform", r:optArg("target-platform"));
                return require("gbs.workspace"):new(Settings):create();
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
        :h(function (r)
                Settings:set("SolutionName"       , solutionName());
                Settings:set("ProjectName"        , r:optArg("name"));
                Settings:set("ProjectType"        , r:optArg("type"));
                Settings:set("ProjectLanguage"    , r:optArg("lang"));
                Settings:set("ProjectDependencies", r:optArg("depends"));
                return require("gbs.project"):new(Settings):create();
           end);

    cli:router()
        :a({"project", "pro", "prj"})
        :b("build")
        :s("name", "")
        :s("config", "debug")
        :s("build-tool", "")
        :s("target-platform", "")
        :h(function (r)
                Settings:set("ProjectName"   , r:optArg("name"));
                Settings:set("BuildConfig"   , r:optArg("config"));
                Settings:set("BuildTool"     , _selectStringValue(r:optArg("build-tool"), "BuildTool"));
                Settings:set("TargetPlatform", _selectStringValue(r:optArg("target-platform"), "TargetPlatform"));
                return require("gbs.project"):new(Settings):build();
           end);

    cli:router()
        :a({"project", "pro", "prj"})
        :b("clean")
        :s("name", "")
        :s("config", "debug")
        :s("build-tool", "")
        :s("target-platform", "")
        :h(function (r)
                Settings:set("ProjectName"   , r:optArg("name"));
                Settings:set("BuildConfig"   , r:optArg("config"));
                Settings:set("BuildTool"     , _selectStringValue(r:optArg("build-tool"), "BuildTool"));
                Settings:set("TargetPlatform", _selectStringValue(r:optArg("target-platform"), "TargetPlatform"));
                return require("gbs.project"):new(Settings):clean();
           end);
        
    cli:router()
        :h(function (r)
                print("Type `gbs help' for usage");
           end);

    if cli:run() then return 0; end
               
    return 1;
end
