local fs  = require("pfs.sys.fs");

local cli = {}

function cli:new (argc, argv)
    local o = {
          _opts    = require("pfs.map"):new()
        , _args    = require("pfs.array"):new()
        , _routers = require("pfs.array"):new()
    }
    self.__index = self;
    return setmetatable(o, self);
end

--function cli:program ()
--    return self._program;
--end

--function cli:programName ()
--    return fs.basename(self._program);
--end

--function cli:opts ()
--    return self._opts;
--end
--
--function cli:args ()
--    return self._args;
--end

function cli:dumpOpts ()
    return tostring(self._opts);
end

function cli:dumpArgs ()
    return tostring(self._args);
end

function cli:hasOpt (optname)
    return self._opts:contains(optname);
end

function cli:optArg (optname)
    return self._opts:at(optname);-- or lib.throw(optname .. ": option must be specified");
end

function cli:insertOpt (optname, optarg)
    self._opts:insert(optname, optarg);
end

function cli:appendArg (a)
    self._args:append(a);
end

function cli:clearOpts ()
    self._opts = require("pfs.map"):new();
end

function cli:clearArgs ()
    self._args = require("pfs.array"):new();
end

function cli.toString (argc, argv)
    local r = argv[1] or "";
    for i = 2, #argv do
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
function cli:parse (argc, argv)
    local i;
    local k = 1;
    
    if not (argc > 0) then
        return true;
    end
    
    self._program = argv[0];
    
    for i = 1, argc do
        if argv[i]:find('-', 1, true) == 1 then
            local optname, optarg = argv[i]:match("^[-][-]?([%a%d_][%a%d_-]-)=(.-)$");
            if optname == nil then
                optname = argv[i]:match("^[-][-]?([%a%d_][%a%d_-]-)$");
                if optname == nil then
                    return false, argv[i] .. ": bad option";
                end
                self._opts:insert(optname, true);
            else
                if not self._opts:contains(optname) then
                    self._opts:insert(optname, optarg);
                else
                    local optval = self._opts:at(optname);
                    
                    if type(optval) ~= "table" then
                        local arr = require("pfs.array"):new();
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

function cli:router ()
    local r = require("pfs.cli.router"):new();
    self._routers:append(r);
    return r;
end

function cli:run ()
    local n = self._routers:size();
    for i = 0, n - 1 do
        local r = self._routers:at(i):_match(self._opts, self._args);
        
        if r then
            if r._h ~= nil then
                if not r._continue then
                    return r._h(r);
                else
                    r._h(r);
                end
            end 
        end
    end
    return false;
end

function cli:guide ()
    local r = require("pfs.cli.guide"):new();
    return r;
end

return cli;
