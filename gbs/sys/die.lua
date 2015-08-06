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

function _object:new (m)
    local o = {
        _m = m
    }; 
    self.__index = self;
    return setmetatable(o, self);
end

function _object:now ()
    local m = self._m;
    local endl = "";

    if m == nil or not m:match("\n$") then
        endl = "\n";
        local s = debug.getinfo(2,'S').source; -- caller source file
        local l = debug.getinfo(2,'l').currentline; -- caller current line
        m = (m or "died") .. " at " .. s .. " line " .. l;
    end
    
    io.stderr:write("ERROR: " .. m .. endl);
    os.exit(-1);
end

function _object:unless (predicate)
    if not predicate then 
        self:now(); 
    end
end

function _object:when (predicate)
    if predicate then 
        self:now(); 
    end
end