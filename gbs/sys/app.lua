local app = {};

local fs = require("gbs.sys.fs");

local _instance = nil;

function app:_instance ()
    if _instance == nil then
        local o = {
            _cmdline = require("gbs.cli.cmdline"):new(#arg, arg);
        };
        self.__index = self;
        _instance = setmetatable(o, self);
    end
    return _instance;
end

function app:cmdline ()
    return self:_instance()._cmdline;
end

return app;