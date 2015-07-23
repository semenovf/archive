local workspace = {};

local lib  = require("gbs.sys.lib");
local fs   = require("gbs.sys.fs");
local app  = require("gbs.sys.app");

function workspace:new (gbs)
    local o = {
        gbs = function () return gbs; end
    }; 
    
    self.__index = self;
    return setmetatable(o, self);
end

function workspace:path ()
    local gbs = self:gbs();
    return gbs:optarg("path") 
        or lib.throw("workspace path must be specified");
end

function workspace:run ()
    local gbs = self:gbs();
    local action = gbs:action() 
        or lib.throw("action for workspace must be specified");

    return lib.runAction(action, {
          create = function () return self:create(); end
--        , build  = function () return self:build(); end
    });
end


function workspace:create ()
    local gbs = self:gbs();
    local path = self:path();
    local workspaceFile = fs.join(path, ".gbs", gbs:workspaceFileName());
    
    if fs.exists(path) then
        lib.print_error(path .. ": can't create workspace: path already exists");
        return false;
    end

    if not (fs.mkdir(path)
            and fs.mkdir(fs.join(path, ".gbs"))
            and fs.mkdir(fs.join(path, ".build"))
            and fs.touch(workspaceFile)) then
        return false;
    end
            
    lib.assert(fs.appendLines(workspaceFile, {
          "#************************************************************"
        , "#* Generated automatically by `" .. app.name() .. "'"
        , "#* Command: `" .. app.cmdline() .. "'"
        , "#* Date:    " .. os.date() 
        , "#************************************************************"
    }));
    
    return true;
end

return workspace;
