require "pfs.string";
require "gbs.utils";

local fs  = require("pfs.sys.fs");

local plugin = {};

function plugin:new (settings)
    local o = {
        _settings = settings
    }; 
    self.__index = self;
    return setmetatable(o, self);
end


-- FIXME
function plugin:_msbuild (solutionName, config, targetPlatform, msbuildTarget)
    local projectName = self:name() or "";

    if string.isEmpty(projectName) then
        fs.execute("msbuild"
            , "/t:" .. msbuildTarget
            , "/p:Configuration=" .. config
            , "/p:Platform=" .. targetPlatform
--            , "/v:quiet"
            , fs.join(".gbs", solutionName .. ".sln"));
    else
--        fs.execute("make", "-f", projectName .. ".make", "-C", ".gbs", "config=" .. config, gmakeTarget);
    end

end


return plugin;
