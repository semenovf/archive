require("gbs.sys.die");

local router = {};
local _routers = require("gbs.sys.array"):new();

function router:new ()
    local o = {
          _actions = require("gbs.sys.array"):new()
        , _opts    = require("gbs.sys.map"):new()
        , _h       = nil -- handler
        , _args    = require("gbs.sys.array"):new() -- free arguments
        , _ropts   = nil -- filled by run() method, contains pairs {optname, optarg}
    }; 
    self.__index = self;
    local r = setmetatable(o, self);
    _routers:append(r);
    return r; 
end

function router:dump ()
    print("Options: " .. tostring(self._opts));
    print("Actions: " .. tostring(self._actions));
end

--
-- Nil value for _dv means option is mandatory. 
-- 
function router:_set (name, valueType, defaultValue)
    self._opts:insert(name, {_type = valueType, _dv = defaultValue});
end

---
-- @param data Must be string or table type
--  
function router:a (data)
    die("Expected string or table type value for action")
        :unless(type(data) == "string" or type(data) == "table");
    self._actions:append(data);
    return self;
end

function router:b (name, defaultValue)
    if defaultValue ~= nil then
        die("Expected boolean value"):unless(type(defaultValue) == "boolean");
    end
    self:_set(name, "b", defaultValue);
    return self;
end

function router:n (name, defaultValue)
    if defaultValue ~= nil then
        die("Expected number value"):unless(type(defaultValue) == "number");
    end
    self:_set(name, "n", defaultValue);
    return self;
end

function router:s (name, defaultValue)
    if defaultValue ~= nil then
        die("Expected string value"):unless(type(defaultValue) == "string");
    end
    self:_set(name, "s", defaultValue);
    return self;
end

function router:h (handler)
    die("Handler must be specified"):when(handler == nil);
    die("Handler must be a function"):unless(type(handler) == "function");
    self._h = handler;
    return self;
end

function router:hasActions ()
    return self._actions:size() > 0;
end

function router:actionAt (index)
    die("Expected number value for arg #1"):unless(type(index) == "number");
    return self._actions:at(index);
end

function router:hasOpt (key)
    return self._ropts:at(key) ~= nil;
end

function router:optArg (key)
    return self._ropts:at(key);
end

function router:_match (opts, args)
    -- Empty router always matches
    if self._actions:size() == 0 and self._opts:size() == 0 then
        return self:new();
    end
    
    -- Expected action(s)
    if self._actions:size() > 0 and args:size() == 0 then
        return nil;
    end

    -- Too few actions
    if self._actions:size() > args:size() then
        return nil;
    end
    
    local i = 0;
    local r = self:new();
    
    while i < self._actions:size() do
        local a = self._actions:at(i);
        
        die("Expected string or table type value for action")
            :unless(type(a) == "string" or type(a) == "table");
        
        local atable = {};
        
        if type(a) == "string" then
            atable = { a };
        else
            atable = a;
        end
        
        local ok = false;
        
        for j = 1, #atable do
            if atable[j] == args:at(i) then
                r._actions:append(atable[j]);
                ok = true;
                break;
            end  
        end
        
        if not ok then break; end
        i = i + 1;
    end
    
    if i ~= self._actions:size() then
        return nil;
    end
    
    local nargs = args:size() - i;
    
    for i = i, args:size() - 1 do
        r._args:append(args:at(i));
    end
    
    local k = nil;
    local v = nil;
    
    r._ropts = require("gbs.sys.map"):new();
    
    for k, v in self._opts:pairs() do
        local ok = false; -- using as flag for successful option matching
        
        if opts:contains(k) then
            local optarg = opts:at(k);
            
            if v._type == "b" then
                if type(optarg) == "boolean" then
                    r._ropts:insert(k, optarg);
                    ok = true;
                end
            elseif v._type == "n" then
                if tonumber(optarg) ~= nil then
                    r._ropts:insert(k, optarg);
                    ok = true;
                end
            elseif v._type == "s" then 
                r._ropts:insert(k, optarg);
                ok = true;
            else
                die("Unexpected type or router may be corrupted"):now();
            end
        elseif v._dv ~= nil then
            r._ropts:insert(k, v._dv);
            ok = true;
        end
        
        -- Option matching failed
        if not ok then
            print("NOT MATCH");
            return nil;
        end
    end
    
    r._h = self._h;
    return r;
end

function router.run (cmdline)
    local n = _routers:size();
    for i = 0, n - 1 do
        local r = _routers:at(i):_match(cmdline:opts(), cmdline:args());
        
        if r then
            if r._h ~= nil then
                return r._h(r);
            end 
        end
    end
    return false;
end

function router.guide ()
    local g = require("gbs.cli.guide"):new();
    return g;
end

return router;
