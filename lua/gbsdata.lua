Gbs = {};

local Lib = require("lib");
local Path = require("path");
require "help";
require "workspace";
--require "domain/solution";
--require "domain/project";

function Gbs:new ()
    local o = {
          opts = {}
    }; 
    self.__index = self;
    return setmetatable(o, self);
end

--- @brief Parses command line arguments
---
--- @param argc Number of command line arguments
--- @param argv Array of command line arguments
--- @return @c true if command line parsed successfully, or @c false on error.
---
function Gbs:parse_opts (argc, argv)
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
                self.opts[optname] = true;
            else
                self.opts[optname] = optarg;
            end
        else
            self.opts[k] = argv[i];
            k = k + 1;
        end
    end

--    print("Free arguments: " .. #self.opts);
--    
--    for key, value in pairs(self.opts) do
--      print(key, value);
--    end

    if #self.opts < 1 then
        Lib.print_error("action must be specified");
        return false;
    end
    
    return true;
end

function Gbs:workspaceFile ()
    return 'workspace.gbs';
end

function Gbs:solutionFile ()
    return 'solution.gbs';
end

function Gbs:domain ()
    if #self.opts > 0 then
        return self.opts[1];
    end
    return nil;
end

function Gbs:action ()
    if #self.opts > 1 then
        return self.opts[2];
    end
    Lib.print_error("action path must be specified");
    return nil;
end

function Gbs:workspacePath ()
    if #self.opts > 2 then
        return self.opts[3];
    end
    Lib.print_error("workspace path must be specified");
    return nil;
end

function Gbs:run ()
    local domain = self:domain();
    local r = true;
    
    if domain == "help" then
        r = self:doHelp();
    elseif domain == "workspace" or domain == "ws" then
        r = self:doWorkspace();
    elseif domain == "solution" then
        r = self:doSolution();
    elseif domain == "project" then
        r = self:doProject();
    else
        Lib.print_error("bad domain");
        r = false;
    end

    if r then return 0; end
    return 1;
end

return Gbs;

--[[
function is_valid_name (name, what)
    return name:match("^([%a%d_-]-)$");
    
    if r == nil then
        print_error("invalid " .. what .. ", ");
        os.exit(1);
    end
end



--]]