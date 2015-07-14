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
    local path = self:workspacePath() or Lib.throw();
    Lib.assert(not Path.exists(path), path .. ": path already exists");
    
    local workspace_filepath = Path.join(path, ".gbs", self:workspaceFile());

    Lib.assert(Path.create_dir(path));
    Lib.assert(Path.create_dir(Path.join(path, ".gbs")));
    Lib.assert(Path.touch(workspace_filepath));
    Lib.assert(File.append_lines(workspace_filepath, {
              "#************************************************************"
            , "#* Generated automatically by " .. App.name()
            , "#* Command: " .. App.cmdline()
            , "#* Date:    " .. os.date() 
            , "#************************************************************"
        }));    
    
    return true;
end
