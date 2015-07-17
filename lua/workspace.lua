local Workspace = {};

local Lib  = require("lib");
local Path = require("path");
local File = require("file");
local App  = require("app");

function Workspace:new (gbs)
    local o = {
        gbs = function () return gbs; end
    }; 
    
    self.__index = self;
    return setmetatable(o, self);
end

function Workspace:path ()
    local gbs = self:gbs();
    return gbs:optarg("path") 
        or Lib.throw("workspace path must be specified");
end

function Workspace:run ()
    local gbs = self:gbs();
    local action = gbs:action() 
        or Lib.throw("action for workspace must be specified");
        
    if action == "create" then
        return self:create();
    else
        Lib.print_error(action .. ": bad action");
        return false;
    end
end


function Workspace:create ()
    local gbs = self:gbs();
    local path = self:path();
    local workspaceFile = Path.join(path, ".gbs", gbs:workspaceFileName());
    
    if Path.exists(path) then
        Lib.print_error(path .. ": can't create workspace: path already exists");
        return false;
    end

    if not (Path.mkdir(path)
            and Path.mkdir(Path.join(path, ".gbs"))
            and Path.mkdir(Path.join(path, ".build"))
            and Path.touch(workspaceFile)) then
        return false;
    end
            
    Lib.assert(File.appendLines(workspaceFile, {
          "#************************************************************"
        , "#* Generated automatically by `" .. App.name() .. "'"
        , "#* Command: `" .. App.cmdline() .. "'"
        , "#* Date:    " .. os.date() 
        , "#************************************************************"
    }));
    
    return true;
end

return Workspace;
