local Solution = {};

local Lib  = require("lib");
local Path = require("path");
local File = require("file");
local App  = require("app");

function Solution:new (gbs)
    local o = {
          name = function ()
                local r = gbs:optarg("name") or Lib.throw("project name must be specified");
                Lib.assert(Lib.is_valid_name(r), "bad project name");
                return r; 
          end
        , action = function () return gbs:action() or Lib.throw("action must be specified"); end
        , workspaceFileName = function () return gbs:workspaceFileName(); end
        , solutionFileName = function () return gbs:solutionFileName(); end
    }; 
    
    self.__index = self;
    return setmetatable(o, self);
end

function Solution:run ()
    local action = self:action() or Lib.throw();
        
    if action == "create" then
        return self:create();
    else
        Lib.print_error(action .. ": bad action");
        return false;
    end
end

function Solution:create ()
    local name = self:name();
    local workspaceFile = Path.join(".gbs", self:workspaceFileName());

    Lib.assert(Path.exists(workspaceFile), "can't create solution outside of workspace directory");
    Lib.assert(Path.mkdir(name), "failed to create solution");
    Lib.assert(Path.mkdir(Path.join(name, ".gbs")));
    Lib.assert(File.appendLines(workspaceFile, { name }));
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

return Solution;