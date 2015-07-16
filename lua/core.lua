Gbs = {};

require "dumper"; -- http://lua-users.org/wiki/DataDumper
local Lib       = require("lib");
local Path      = require("path");
local Help      = require("help");
local Workspace = require("workspace");
local Solution  = require("solution");
local Project   = require("project");

-- Define a shortcut function for testing
function dump (...)
    print(DataDumper(...));
end

function Gbs:new ()
    local o = {
          _opts = {}
        , homeDir           = function () return os.getenv("GBS_HOME"); end 
        , solutionFileName  = function () return 'solution.gbs'; end
        , workspaceFileName = function () return 'workspace.txt'; end
        
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
                if self._opts[optname] == nil then
                    self._opts[optname] = optarg;
                else
                    if type(self._opts[optname]) ~= "table" then 
                        local x = self._opts[optname];
                        self._opts[optname] = {};
                        table.insert(self._opts[optname], x);
                    end
                    table.insert(self._opts[optname], optarg);
                end
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
    return nil;
end

function Gbs:hasOpt (optname)
    return self._opts[optname] ~= nil;
end

function Gbs:optarg (optname)
    return self._opts[optname] or Lib.throw(optname .. ": option must be specified");
end

function Gbs:getSolutionNameFromFile(solutionFile)
    local r = nil;
    for line in io.lines(solutionFile) do
        r = line:match('^solution%s"([^%s]-)"');
        if r ~= nil then
            break;
        end
    end
    return r;
end

function Gbs:run ()
    if self:hasOpt("dump") then
        dump(self._opts);
        return 0;
    end

    local domain = self:domain();
    local r = true;
   
    if domain == "help" then
        local help = Help:new();
        help:usage();
    elseif domain == "workspace" or domain == "ws" then
        local ws = Workspace:new(self);
        r = ws:run();
    elseif domain == "solution" or domain == "sln" then
        local sln = Solution:new(self);
        r = sln:run();
    elseif domain == "project"  or domain == "pro" then
        local pro = Project:new(self);
        r = pro:run();
    else
        Lib.print_error("bad domain or it must be specified");
        r = false;
    end

    if r then return 0; end
    return 1;
end

return Gbs;
