local cmdline = {}

local fs  = require("gbs.sys.fs");
local lib = require("gbs.sys.lib");

function cmdline:new (argc, argv)
    local o = {
          _opts    = require("gbs.sys.map"):new()
        , _args    = require("gbs.sys.array"):new()
        , _argc    = argc
        , _argv    = argv
    }
    self.__index = self;
    return setmetatable(o, self);
end

function cmdline:path ()
    return self._argv[0];
end

function cmdline:program ()
    return fs.basename(self._argv[0]);
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

function cmdline:tostring ()
    local r = self:name();
    for i = 1, #self._argv do
        r = r .. ' ' .. self._argv[i]; 
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
function cmdline:parse ()
    local i;
    local k = 1;
    
    for i = 1, self._argc do
        if self._argv[i]:find('--', 1, true) == 1 then
            local optname, optarg = self._argv[i]:match("^[-][-]([%a%d_-]-)=(.-)$");
            if optname == nil then
                optname = self._argv[i]:match("^[-][-]([%a%d_-]-)$");
                if optname == nil then
                    lib.print_error(self._argv[i] .. ": bad option");
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
            self._args:append(self._argv[i]);
        end
    end

--    print("Options: " .. tostring(self._opts));
--    print("Free arguments: " .. tostring(self._args));
    return true;
end

return cmdline;
