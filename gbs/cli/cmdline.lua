local cmdline = {}

local fs  = require("gbs.sys.fs");
local lib = require("gbs.sys.lib");

function cmdline:new (argc, argv)
    local o = {
          _opts = require("gbs.sys.map"):new()
        , _args = require("gbs.sys.array"):new()
        , _program = ""
    }
    self.__index = self;
    return setmetatable(o, self);
end

function cmdline:program ()
    return self._program;
end

function cmdline:programName ()
    return fs.basename(self._program);
end

function cmdline:opts ()
    return self._opts;
end

function cmdline:args ()
    return self._args;
end

function cmdline:hasOpt (optname)
    return self._opts:contains(optname);
end

function cmdline:optArg (optname)
    return self._opts:at(optname);-- or lib.throw(optname .. ": option must be specified");
end

function cmdline:insertOpt (optname, optarg)
    self._opts:insert(optname, optarg);
end

function cmdline:appendArg (a)
    self._args:append(a);
end

function cmdline:clearOpts ()
    self._opts = require("gbs.sys.map"):new();
end

function cmdline:clearArgs ()
    self._args = require("gbs.sys.array"):new();
end

function cmdline.tostring (argc, argv)
    local r = argv[0];
    for i = 1, #argv do
        r = r .. ' ' .. argv[i]; 
    end
    return r;
end    

---
--- @brief Parses command line arguments
---
--- @param argc Number of command line arguments
--- @param argv Array of command line arguments
--- @return @c true if command line parsed successfully, or @c false on error.
---
function cmdline:parse (argc, argv)
    local i;
    local k = 1;
    
    if not (argc > 0) then
        return true;
    end
    
    self._program = argv[0];
    
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

return cmdline;
