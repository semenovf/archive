--- 
-- Examples:
-- 
-- die():now()              -- ERROR: died at @path/to/script.lua line N 
-- die("Died"):when(true)   -- ERROR: Died at @path/to/script.lua line N
-- die("Died\n"):when(true) -- ERROR: Died
-- die():unless(false)      -- ERROR: died at @path/to/script.lua line N
--

local _object = {}

function die (m)
    return _object:new((type(m) == "string" and m) or nil);
end

function throw (m)
    error(m);
end

function print_error (m)
    if m ~= nil then
        io.stderr:write("ERROR: " .. tostring(m) .. "\n");
    end    
end

function print_warn (m)
    if m ~= nil then
        io.stderr:write("WARN: " .. tostring(m) .. "\n");
    end    
end

function _object:new (m)
    local o = {
        _m = m
    }; 
    self.__index = self;
    return setmetatable(o, self);
end

function _object:_print ()
    local m = self._m;
    local endl = "";

    if m == nil or not m:match("\n$") then
        endl = "\n";
        local s = debug.getinfo(3,'S').source; -- caller source file
        local l = debug.getinfo(3,'l').currentline; -- caller current line
        m = (m or "died") .. " at " .. s .. " line " .. l;
    end
    
    io.stderr:write("ERROR: " .. m .. endl);
    os.exit(-1);
end

function _object:now ()
    self:_print();
end

function _object:unless (predicate)
    if not predicate then 
        self:_print(); 
    end
end

function _object:when (predicate)
    if predicate then 
        self:_print();
    end
end
