local Lib  = require("lib");
local Path = require("path");
local File = require("file");
local App  = require("app");

function Gbs:doSolution ()
    local action = self:action() or Lib.throw();
        
    if action == "create" then
        return self:createSolution();
    else
        Lib.print_error(action .. ": bad action");
        return false;
    end
    
    return true;
end

function Gbs:createSolution ()
    local name = self:solutionName() or Lib.throw("solution name must be specified");
    Lib.assert(Lib.is_valid_name(name), "bad solution name");
    
    local workspaceFilepath = Path.join(".gbs", self:workspaceFile());

    Lib.assert(Path.exists(workspaceFilepath), "can't create solution outside of workspace directory");
    Lib.assert(Path.mkdir(name), "failed to create solution");
    Lib.assert(Path.mkdir(Path.join(name, ".gbs")));
    Lib.assert(File.appendLines(workspaceFilepath, { name }));
    Lib.assert(File.appendLines(Path.join(name, ".gbs", "solution.gbs"), {
              "--************************************************************"
            , "--* Generated automatically by `" .. App.name() .. "'"
            , "--* Command: `" .. App.cmdline() .. "'"
            , "--* Date:    " .. os.date() 
            , "--************************************************************"
            , 'solution ' .. Lib.quote(name)
            , '    configurations {"debug", "release"}'
        }));    
    
    return true;
end