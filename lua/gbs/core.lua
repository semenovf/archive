require "pfs.die";

local fs = require("pfs.sys.fs");
local Settings = require("pfs.settings"):new("gbs.");

Settings:set("HomeDir"          , os.getenv("GBS_HOME"));
Settings:set("SolutionFileName" , "solution.gbs");
Settings:set("WorkspaceFileName", "workspace.txt");
Settings:set("ProjectFileName"  , "project.gbs");
Settings:set("SourcesDirName"   , "src");
Settings:set("TestsDirName"     , "tests");

gbs = {};

local _instance = nil;

--function gbs.instance ()
--    if not _instance then
--        _instance = gbs;
----        _instance._argc             = #arg;
----        _instance._argv             = arg;
----        _instance._cmdline          = require("gbs.cli.cmdline"):new();
----        _instance._cmdlineString    = _instance._cmdline.toString(_instance._argc, _instance._argv);
--        
----        _instance.argc              = function () return _instance._argc; end
----        _instance.argv              = function () return _instance._argv; end
--        _instance.cmdline           = function () return _instance._cmdline; end
--        _instance.programName       = function () return _instance._cmdline:programName(); end 
--        _instance.cmdlineString     = function () return _instance._cmdlineString; end
--        _instance.loadPreferences   = function () 
--            return _loadPreferences(_instance.workspaceFileName()
--                , _instance._cmdline:opts());
--        end
--        _instance.solutionName      = function () return _solitionName(_instance.solutionFileName); end
--        _instance.run               = function () return _run(); end
--    end 
--    return _instance;
--end

function _loadPreferences (workspaceFileName, opts)
    local g = gbs.instance();
    local workspaceFile = fs.join(".gbs", workspaceFileName);
    
    for i = 1, 4 do
        if fs.exists(workspaceFile) then
            break;
        end
        workspaceFile = fs.join("..", workspaceFile);
    end
    
    if not fs.exists(workspaceFile) then
        return false;
    end

    for line in io.lines(workspaceFile) do
        if not line:match('^#') then
            local k, v = line:match('^([^=]-)=(.-)$');
            if k ~= nil then
                opts:insert(k, v);
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

function _solutionName (solutionFileName)
    local solutionFile = fs.join(".gbs", solutionFileName);
    
    die(solutionFile .. ": solution file not found"):unless(fs.exists(solutionFile));
    local solutionName = _getSolutionNameFromFile(solutionFile);
    die(solutionFile .. ": unable to take solution name, solution file may be corrupted")
        :when(solutionName == nil);
    return solutionName;
end

--function gbs:runDeprecated ()
--    if self:hasOpt("dump") then
--        print("Options: " .. tostring(self._opts));
--        print("Free arguments: " .. tostring(self._args));
--        return 0;
--    end
--
--    local domain = self:domain();
--    local r = true;
--   
--    if domain == "help" then
--        local help = require("gbs.help"):new();
--        
--        if self._args:size() > 1 then
--            help:help(self._args:at(1));
--        else
--            help:usage();
--        end
--    elseif domain == "workspace" or domain == "ws" then
--        local ws = require("gbs.workspace"):new(self);
--        r = ws:run();
--    elseif domain == "solution" 
--            or domain == "sln" then
--        local sln = require("gbs.solution"):new(self);
--        r = sln:run();
--    elseif domain == "project" 
--            or domain == "pro"
--            or domain == "prj" then
--        local pro = require("gbs.project"):new(self);
--        r = pro:run();
--    else
--        lib.print_error("bad domain or it must be specified (try `gbs help')");
--        r = false;
--    end
--
--    if r then return 0; end
--    return 1;
--end


function gbs.run (argc, argv)
    local cli = require("pfs.cli.cli"):new();
    
    local status, msg = cli:parse(#arg, arg);
    die(msg):unless(status);

 

    --instance.loadPreferences();
    
--    local router_type = require("gbs.cli.router");
    local help_type = require("gbs.help");

    cli:router()
        :b("dump")
        :h(function (r)
                print("Options: " .. tostring(cli:opts()));
                print("Free arguments: " .. tostring(cli:args()));
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
        :s("target-platform")
        :h(function (r)
                local ws = require("gbs.workspace"):new();
                return ws:create(r);
           end);

    cli:router()
        :a({"solution", "sln"})
        :b("create")
        :s("name")
        :h(function (r)
                local sln = require("gbs.solution"):new();
                sln:setName(r.optArg("name"));
                return sln:create(r);
           end);
        
    cli:router()
        :h(function (r)
                print_error("Try `gbs help' for usage");
           end);

    if cli:run() then return 0; end
               
--    local domain = self:domain();
--    local r = true;
--   
--    if domain == "help" then
--        local help = require("gbs.help"):new();
--        
--        if self._args:size() > 1 then
--            help:help(self._args:at(1));
--        else
--            help:usage();
--        end
--    elseif domain == "workspace" or domain == "ws" then
--        local ws = require("gbs.workspace"):new(self);
--        r = ws:run();
--    elseif domain == "solution" 
--            or domain == "sln" then
--        local sln = require("gbs.solution"):new(self);
--        r = sln:run();
--    elseif domain == "project" 
--            or domain == "pro"
--            or domain == "prj" then
--        local pro = require("gbs.project"):new(self);
--        r = pro:run();
--    else
--        lib.print_error("bad domain or it must be specified (try `gbs help')");
--        r = false;
--    end
--
--    if r then return 0; end
    return 1;
end
