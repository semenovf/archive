local array = {};

--
-- indexing from zero (not from 1)
--
function array:new ()
    local o = {
        _data = {}
      , _count = 0
    }; 
    self.__index = self;
    self.__tostring = _array_tostring;
    return setmetatable(o, self);
end

---
-- @brief Returns number of elements in the array.
-- 
-- @return Number of elements in the array
-- 
function array:size ()
    return self._count;
end


---
-- @brief Synonym for size() method
-- 
-- @return Number of elements in the array 
--
function array:count ()
    return self._count;
end

function array:at (pos)
    if type(pos) ~= "number" then
        error("expected number as `pos' value");
    end
    if pos < 0 or pos >= self._count then
        error("`pos' is out of bounds"); 
    end
    
    return self._data[pos + 1];
end

function array:insert (pos, value)
    if type(pos) ~= "number" then
        error("expected number as `pos' value");
    end
    
    if pos < 0 then pos = 0; end
    if pos >= self._count then pos = self._count; end
    
    pos = pos + 1;
    
    table.insert(self._data, pos, value);
    self._count = self._count + 1; 
end

function array:prepend (value)
    self:insert(0, value);
end

function array:append (value)
    self:insert(self._count, value);
end

function _array_tostring (a)
    local r = "[";
    local separator = "";
    for i = 0, a:size() - 1 do
        r = r .. separator .. tostring(a._data[i + 1]);
        separator = ", ";
    end
    r = r .. "]";
    return r;
end

return array;
