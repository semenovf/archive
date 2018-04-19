local map = {};

function map:new ()
    local o = {
        _data = {}
      , _count = 0
    }; 
    self.__index = self;
    self.__tostring = _map_tostring;
    return setmetatable(o, self);
end

---
-- @brief Returns number of elements in the array.
-- 
-- @return Number of elements in the array
-- 
function map:size ()
    return self._count;
end

---
-- @brief Returns native data.
-- 
-- @return Native data.
-- 
function map:data ()
    return self._data;
end

---
-- @brief Synonym for size() method
-- 
-- @return Number of elements in the array 
--
function map:count ()
    return self._count;
end

function map:at (key)
    return self._data[key];
end

function map:insert (key, value)
    if self._data[key] == nil then
        self._data[key] = value;
        self._count = self._count + 1;
    else
        self._data[key] = value;
    end 
end

function map:remove (key)
    if self._data[key] ~= nil then
        self._data[key] = nil;
        self._count = self._count - 1;
    end 
end

function map:contains (key)
    return self._data[key] ~= nil and true or false;
end

function map:pairs ()
    return pairs(self._data); 
end

function _map_tostring (m)
    local r = "{";
    local separator = "";
    for k, v in pairs(m._data) do
        r = r .. separator .. k .. ": " .. tostring(v);
        separator = "; ";
    end
    r = r .. "}";
    return r;
end

return map;