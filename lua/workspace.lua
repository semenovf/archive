local Workspace = {};

local Lib  = require("lib");
local Path = require("path");
local File = require("file");
local App  = require("app");

function Workspace:new (gbs)
    local o = {
          action = function () return gbs:action() or Lib.throw("action must be specified"); end
        , path = function () return gbs:optarg("path") or Lib.throw("workspace path must be specified"); end
        , workspaceFileName = function () return gbs:workspaceFileName(); end
    }; 
    
    self.__index = self;
    return setmetatable(o, self);
end

function Workspace:run ()
    local action = self:action() or Lib.throw();
        
    if action == "create" then
        return self:create();
    else
        Lib.print_error(action .. ": bad action");
        return false;
    end
end


function Workspace:create ()
    local path = self:path();
    Lib.assert(not Path.exists(path), path .. ": path already exists");
    
    local workspaceFile = Path.join(path, ".gbs", self:workspaceFileName());

    Lib.assert(Path.mkdir(path));
    Lib.assert(Path.mkdir(Path.join(path, ".gbs")));
    Lib.assert(Path.mkdir(Path.join(path, ".build")));
    Lib.assert(Path.touch(workspaceFile));
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