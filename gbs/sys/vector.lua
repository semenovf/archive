
function vector:new ()
    local o = {_data = {}
        , _count = 0 }; 
    self.__index = self;
    return setmetatable(o, self);
end

function vector:count ()
    return self._count;
end

function vector:capacity ()
    return self:getn();
end

function vector:size ()
    return self._count;
end

function vector:at (pos)
    if type(pos) ~= "number" then
        error("expected number as `pos' value");
    end
    if pos < 1 or pos > _count then
        error("`pos' is oyt of bounds"); 
    end
    
    return self._data[pos];
end

function vector:insert (pos, value)
    if type(pos) ~= "number" then
        error("expected number as `pos' value");
    end
    if pos < 1 then pos = 1; end
    if pos > _count then pos = _count; end
    
    table.insert(self._data, pos, value);
    self._count = self._count + 1; 
end

function array:prepend (value)
    self:insert(1, value);
end

function array:append (value)
    self:insert(pos + 1, value);
end