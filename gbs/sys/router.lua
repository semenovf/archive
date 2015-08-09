require("gbs.sys.die");

local router = {};
local _routers = require("gbs.sys.array"):new();

function router:new ()
    local o = {
          _actions = require("gbs.sys.array"):new()
        , _opts    = require("gbs.sys.map"):new()
        , _h       = function (r) die("Handler must be specified"):now(); end
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
    self._run = handler;
    return self;
end

function router:actionAt (index)
    die("Expected number value for arg #1"):unless(type(index) == "number");
    self._actions:at(index);
    return self;
end

function router.run(opts, args)
    local n = _routers:size();
    for i = 0, n - 1 do
        local r = _routers:at(i);
    end
end

return router;
