require("gbs.sys.die");

local router = {};
--local _routers = require("gbs.sys.array"):new();

function router:new ()
    local o = {
          _opts    = require("gbs.sys.map"):new()
        , _actions = require("gbs.sys.array"):new()
    }; 
    self.__index = self;
    local r = setmetatable(o, self);
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

function router:a (name)
    die("Expected string value for arg #1"):unless(type(name) == "string");
    self._actions:append(name);
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

return router;