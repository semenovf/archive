Gbs = {};

local Lib = require("lib");
local Path = require("path");
require "help";
require "workspace";
require "solution";
local Project = require("project");

function Gbs:new ()
    local o = {
            _opts = {}
          , workspaceFile = function () return 'workspace.txt'; end
          , solutionFile  = function () return 'solution.gbs'; end
    }; 
    self.__index = self;
    return setmetatable(o, self);
end

---
--- @brief Parses command line arguments
---
--- @param argc Number of command line arguments
--- @param argv Array of command line arguments
--- @return @c true if command line parsed successfully, or @c false on error.
---
function Gbs:parseCommandLine (argc, argv)
    local i;
    local k = 1;
    
    for i = 1, argc do
        if argv[i]:find('--', 1, true) == 1 then
            local optname, optarg = argv[i]:match("^[-][-]([%a%d_-]-)=(.-)$");
            if optname == nil then
                optname = argv[i]:match("^[-][-]([%a%d_-]-)$");
                if optname == nil then
                    Lib.print_error(argv[i] .. ": bad option");
                    return false;
                end
                self._opts[optname] = true;
            else
                self._opts[optname] = optarg;
            end
        else
            self._opts[k] = argv[i];
            k = k + 1;
        end
    end

--    print("Free arguments: " .. #self._opts);
--    
--    for key, value in pairs(self._opts) do
--      print(key, value);
--    end

    if #self._opts < 1 then
        Lib.print_error("domain must be specified");
        return false;
    end
    
    return true;
end

function Gbs:domain ()
    if #self._opts > 0 then
        return self._opts[1];
    end
    return nil;
end

function Gbs:action ()
    if #self._opts > 1 then
        return self._opts[2];
    end
    Lib.print_error("action must be specified");
    return nil;
end

function Gbs:workspacePath ()
    if #self._opts > 2 then
        return self._opts[3];
    end
    return nil;
end

function Gbs:solutionName ()
    if #self._opts > 2 then
        return self._opts[3];
    end
    return nil;
end

function Gbs:getSolutionNameFromFile(solutionFilepath)
    local r = nil;
    for line in io.lines(solutionFilepath) do
        r = line:match('^solution%s"([^%s]-)"');
        if r ~= nil then
            break;
        end
    end
    return r;
end

function Gbs:newProject ()
{
    local solutionName = nil;
    local solutionFilepath = Path.join(".gbs", self:solutionFileName());
    
    if Path.exists(solutionFilepath) then
        solutionName = getSolutionNameFromFile(solutionFilepath)
            or Lib.throw(solutionFilepath .. ": unable to take solution name, may be solution file is absent or corrupted");
    end
    
    local project = Project:new();
    project._name = self._opts[name];
    project._solutionName = solutionName;
    
    return project;
}

function Gbs:run ()
    local domain = self:domain();
    local r = true;
    
    if domain == "help" then
        r = self:doHelp();
    elseif domain == "workspace" or domain == "ws" then
        local project = self:newProject();
        r = project:run();
    elseif domain == "solution" or domain == "sln" then
        r = self:doSolution();
    elseif domain == "project"  or domain == "pro" then
        r = self:doProject();
    else
        Lib.print_error("bad domain");
        r = false;
    end

    if r then return 0; end
    return 1;
end

return Gbs;
