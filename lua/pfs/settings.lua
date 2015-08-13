require "pfs.die";

local settings = {};

---
-- @brief Creates new settings instance
-- 
-- @param key_prefix (optional) Sets key prefix for key name
---
function settings:new (key_prefix)
    self.__index = self;
    return setmetatable(
        { _key_prefix = key_prefix or "." }
        , self);
end

---
-- @brief Associates value @c value with key @c key.
-- 
-- @param key Setting key.
-- @param value Value to be set for entry associated with key @ key. 
--        If value equals to @c nil then entry will be removed 
--        from the table. 
---
function settings:set (key, value)
    self[self._key_prefix .. key] = value;
end

---
-- @brief Removes entry associated by key @ key
-- 
-- @param key Key for entry to be removed.
---
function settings:remove (key)
    self[self._key_prefix .. key] = nil;
end

---
-- @brief Get value for entry associated with key @ key.
-- 
-- @param key Key for entry to get value.
--
---
function settings:get (key, defaultValue)
    return self[self._key_prefix .. key] or defaultValue;
end

function settings:get_or_throw (key, defaultValue)
    local r = self[self._key_prefix .. key];
    if r == nil then r = defaultValue; end
    if r == nil then throw_undefined(key); end
    return r;
end

return settings;