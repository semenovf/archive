local vector = {};

--
-- indexing from zero (not from 1)
--
function vector:new ()
    local o = {
        _data = {}
      , _count = 0
    }; 
    self.__index = self;
    self.__tostring = _vector_tostring;
    return setmetatable(o, self);
end

---
-- @brief Returns number of elements in the vector.
-- 
-- @return Number of elements in the vector
-- 
function vector:size ()
    return self._count;
end

---
-- @brief Returns native data.
-- 
-- @return Native data.
-- 
function vector:data ()
    return self._data;
end

---
-- @brief Synonym for size() method
-- 
-- @return Number of elements in the vector 
--
function vector:count ()
    return self._count;
end

function vector:at (pos)
    if type(pos) ~= "number" then
        error("expected number as `pos' value");
    end
    if pos < 0 or pos >= self._count then
        error("`pos' is out of bounds"); 
    end
    
    return self._data[pos + 1];
end

function vector:insert (pos, value)
    if type(pos) ~= "number" then
        error("expected number as `pos' value");
    end
    
    if pos < 0 then pos = 0; end
    if pos >= self._count then pos = self._count; end
    
    pos = pos + 1;
    
    table.insert(self._data, pos, value);
    self._count = self._count + 1; 
end

function vector:prepend (value)
    self:insert(0, value);
end

function vector:append (value)
    self:insert(self._count, value);
end

function vector:push_back (value)
    self:insert(self._count, value);
end

function _vector_tostring (a)
    local r = "[";
    local separator = "";
    for i = 0, a:size() - 1 do
        r = r .. separator .. tostring(a._data[i + 1]);
        separator = ", ";
    end
    r = r .. "]";
    return r;
end

return vector;
