local gbs = {};

local lib   = require("gbs.sys.lib");
local fs    = require("gbs.sys.fs");

function gbs:_instance ()
    return require("gbs.sys.app"):_instance();
end

function gbs:homeDir ()           return os.getenv("GBS_HOME"); end 
function gbs:solutionFileName ()  return 'solution.gbs'; end
function gbs:workspaceFileName () return 'workspace.txt'; end
function gbs:projectFileName ()   return 'project.gbs'; end
function gbs:sourcesDirName ()    return 'src'; end
function gbs:testsDirName ()      return 'tests'; end

function gbs:loadPrefs ()

    local workspaceFile = fs.join(".gbs", self:workspaceFileName());
    
--    print("Searching " .. workspaceFile .. " ...");
    
    for i = 1, 4 do
        if fs.exists(workspaceFile) then
--            print("... " .. workspaceFile .. ": file found");
            break;
        end
        workspaceFile = fs.join("..", workspaceFile);
    end
    
    if not fs.exists(workspaceFile) then
--        print("... " .. self:workspaceFileName() .. " not found");
        return false;
    end

    for line in io.lines(workspaceFile) do
        if not line:match('^#') then
            local key, value = line:match('^([^=]-)=(.-)$');
            if key ~= nil then
                self._opts:insert(key, value);
            end
        end
    end
end

function _getSolutionNameFromFile(solutionFile)
    local r = nil;
    for line in io.lines(solutionFile) do
        r = line:match('^solution%s"([^%s]-)"');
        if r ~= nil then
            break;
        end
    end
    return r;
end

function gbs:solutionName ()
    local solutionFilePath = fs.join(".gbs", self:solutionFileName());
    
    if not fs.exists(solutionFilePath) then
        lib.throw(solutionFilePath .. ": solution file not found");
    end

    local solutionName = _getSolutionNameFromFile(solutionFilePath)
            or lib.throw(solutionFilePath .. ": unable to take solution name, solution file may be corrupted");
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


function gbs:run ()

--    Gbs:loadPrefs();
    local cmdline = self:cmdline();
    
    if not cmdline:parse() then
        return -1;
    end

    if cmdline:hasOpt("dump") then
        print("Options: " .. tostring(self._opts));
        print("Free arguments: " .. tostring(self._args));
        return 0;
    end

    local router_type = require("gbs.cli.router");
    local help_type = require("gbs.help");

    router_type:new()
        :a("help")
        :a({"workspace", "ws", "solution", "sln", "project", "pro", "prj"})
        :h(function (r)
                help_type:new():help(r:actionAt(1));
                return true;
           end);
    
    router_type:new()
        :a("help")
        :h(function (r) 
                router_type.guide():usage();
                return true;
           end);
           
    router_type:new()
        :a({"workspace", "ws"})
        :b("create")
        :s("path")
        :s("build-tool")
        :s("target-platform")
        :h(function (r)
                local ws = require("gbs.workspace"):new();
                return ws:create();
           end);

    router_type:new()
        :a({"solution", "sln"})
        :b("create")
        :s("name")
        :h(function (r)
                local sln = require("gbs.solution"):new();
                sln:setName(r.optArg("name"));
                return sln:create();
           end);
        
    router_type:new()
        :h(function (r)
                lib.print_error("Try `gbs help' for usage");
           end);

    if router_type.run(cmdline) then return 0; end
               
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

return gbs;
