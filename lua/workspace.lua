local Lib  = require("lib");
local Path = require("path");
local File = require("file");
local App  = require("app");

function Gbs:doWorkspace ()
    local action = self:action() or Lib.throw();
        
    if action == "create" then
        return self:createWorkspace();
    else
        Lib.print_error(action .. ": bad action");
        return false;
    end
    
    return true;
end


function Gbs:createWorkspace ()
    local path = self:workspacePath() or Lib.throw("workspace path must be specified");
    
    Lib.assert(not Path.exists(path), path .. ": path already exists");
    
    local workspaceFilepath = Path.join(path, ".gbs", self:workspaceFile());

    Lib.assert(Path.mkdir(path));
    Lib.assert(Path.mkdir(Path.join(path, ".gbs")));
    Lib.assert(Path.mkdir(Path.join(path, ".build")));
    Lib.assert(Path.touch(workspaceFilepath));
    Lib.assert(File.appendLines(workspaceFilepath, {
              "#************************************************************"
            , "#* Generated automatically by `" .. App.name() .. "'"
            , "#* Command: `" .. App.cmdline() .. "'"
            , "#* Date:    " .. os.date() 
            , "#************************************************************"
        }));    
    
    return true;
end
