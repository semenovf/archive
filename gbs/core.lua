local gbs = {};

local lib   = require("gbs.sys.lib");
local fs    = require("gbs.sys.fs");

function gbs:new ()
    local o = {
          _opts  = require("gbs.sys.map"):new()
        , _args  = require("gbs.sys.array"):new()
        , homeDir           = function () return os.getenv("GBS_HOME"); end 
        , solutionFileName  = function () return 'solution.gbs'; end
        , workspaceFileName = function () return 'workspace.txt'; end
--        , buildToolFileName = function () return 'build-tool.txt'; end
--        , platformFileName  = function () return 'platform.txt'; end
        , projectFileName   = function () return 'project.gbs'; end
        , sourcesDirName    = function () return 'src'; end
        , testsDirName      = function () return 'tests'; end
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
                self._opts:insert(optname, true);
            else
                if not self._opts:contains(optname) then
                    self._opts:insert(optname, optarg);
                else
                    local optval = self._opts:at(optname);
                    
                    if type(optval) ~= "table" then
                        local arr = require("gbs.sys.array"):new();
                        arr:append(optval);
                        self._opts:insert(optname, arr);
                        optval = self._opts:at(optname);
                    end
                    optval:append(optarg);    
                end
            end
        else
            self._args:append(argv[i]);
        end
    end

--    print("Options: " .. tostring(self._opts));
--    print("Free arguments: " .. tostring(self._args));
    return true;
end

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

function gbs:domain ()
    if self._args:size() > 0 then
        return self._args:at(0);
    end
    return nil;
end

function gbs:hasOpt (optname)
    return self._opts:contains(optname);
end

function gbs:optarg (optname)
    return self._opts:at(optname);-- or lib.throw(optname .. ": option must be specified");
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


function gbs:runDeprecated ()
    if self:hasOpt("dump") then
        print("Options: " .. tostring(self._opts));
        print("Free arguments: " .. tostring(self._args));
        return 0;
    end

    local domain = self:domain();
    local r = true;
   
    if domain == "help" then
        local help = require("gbs.help"):new();
        
        if self._args:size() > 1 then
            help:help(self._args:at(1));
        else
            help:usage();
        end
    elseif domain == "workspace" or domain == "ws" then
        local ws = require("gbs.workspace"):new(self);
        r = ws:run();
    elseif domain == "solution" 
            or domain == "sln" then
        local sln = require("gbs.solution"):new(self);
        r = sln:run();
    elseif domain == "project" 
            or domain == "pro"
            or domain == "prj" then
        local pro = require("gbs.project"):new(self);
        r = pro:run();
    else
        lib.print_error("bad domain or it must be specified (try `gbs help')");
        r = false;
    end

    if r then return 0; end
    return 1;
end


function gbs:run ()
    if self:hasOpt("dump") then
        print("Options: " .. tostring(self._opts));
        print("Free arguments: " .. tostring(self._args));
        return 0;
    end

    local domain = self:domain();
    local r = true;
   
    if domain == "help" then
        local help = require("gbs.help"):new();
        
        if self._args:size() > 1 then
            help:help(self._args:at(1));
        else
            help:usage();
        end
    elseif domain == "workspace" or domain == "ws" then
        local ws = require("gbs.workspace"):new(self);
        r = ws:run();
    elseif domain == "solution" 
            or domain == "sln" then
        local sln = require("gbs.solution"):new(self);
        r = sln:run();
    elseif domain == "project" 
            or domain == "pro"
            or domain == "prj" then
        local pro = require("gbs.project"):new(self);
        r = pro:run();
    else
        lib.print_error("bad domain or it must be specified (try `gbs help')");
        r = false;
    end

    if r then return 0; end
    return 1;
end

return gbs;
