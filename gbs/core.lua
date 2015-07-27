local gbs = {};

require "gbs.sys.dumper"; -- http://lua-users.org/wiki/DataDumper
local lib   = require("gbs.sys.lib");
local fs    = require("gbs.sys.fs");

-- Define a shortcut function for testing
function dump (...)
    print(DataDumper(...));
end

function gbs:new ()
    local o = {
          _opts = {}
        , homeDir           = function () return os.getenv("GBS_HOME"); end 
        , solutionFileName  = function () return 'solution.gbs'; end
        , workspaceFileName = function () return 'workspace.txt'; end
        , premakeActionFileName = function () return 'premake-action.txt'; end
        , platformFileName  = function () return 'platform.txt'; end
        , projectFileName   = function () return 'project.gbs'; end
        , sourcesDirName    = function () return 'src'; end
        , testsDirName      = function () return 'tests'; end
    }; 
    self.__index = self;
    return setmetatable(o, self);
end

--function binDir ()
--end
--
--function libDir ()
--end
--
--function includeDir ()
--end



---
--- @brief Parses command line arguments
---
--- @param argc Number of command line arguments
--- @param argv Array of command line arguments
--- @return @c true if command line parsed successfully, or @c false on error.
---
function gbs:parseCommandLine (argc, argv)
    local i;
    local k = 1;
    
    for i = 1, argc do
        if argv[i]:find('--', 1, true) == 1 then
            local optname, optarg = argv[i]:match("^[-][-]([%a%d_-]-)=(.-)$");
            if optname == nil then
                optname = argv[i]:match("^[-][-]([%a%d_-]-)$");
                if optname == nil then
                    lib.print_error(argv[i] .. ": bad option");
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

function gbs:domain ()
    if #self._opts > 0 then
        return self._opts[1];
    end
    return nil;
end

function gbs:action ()
    if #self._opts > 1 then
        return self._opts[2];
    end
    return nil;
end

function gbs:hasOpt (optname)
    return self._opts[optname] ~= nil;
end

function gbs:optarg (optname)
    return self._opts[optname];-- or lib.throw(optname .. ": option must be specified");
end

function gbs:getSolutionNameFromFile(solutionFile)
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

    local solutionName = self:getSolutionNameFromFile(solutionFilePath)
            or lib.throw(solutionFilePath .. ": unable to take solution name, solution file may be corrupted");
    return solutionName;
end


function gbs:run ()
    if self:hasOpt("dump") then
        dump(self._opts);
        return 0;
    end

    local domain = self:domain();
    local r = true;
   
    if domain == "help" then
        local help = require("gbs.help"):new();
        help:usage();
    elseif domain == "workspace" or domain == "ws" then
        local ws = require("gbs.workspace"):new(self);
        r = ws:run();
    elseif domain == "solution" or domain == "sln" then
        local sln = require("gbs.solution"):new(self);
        r = sln:run();
    elseif domain == "project"  or domain == "pro" then
        local pro = require("gbs.project"):new(self);
        r = pro:run();
    else
        lib.print_error("bad domain or it must be specified");
        r = false;
    end

    if r then return 0; end
    return 1;
end

return gbs;
