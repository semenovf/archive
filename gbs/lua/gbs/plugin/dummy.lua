local plugin = {};

function plugin:new (settings)
    local o = {}; 
    self.__index = self;
    return setmetatable(o, self);
end

function plugin:transaction ()
    return nil;
end

return plugin;
